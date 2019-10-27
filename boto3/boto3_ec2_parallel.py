#!/usr/bin/python3

import time
import boto3
import argparse

blockDeviceMappings = [{
    "DeviceName": "/dev/xvda",
    "Ebs": {
        "SnapshotId": "snap-08a7cb489033af8c7",
        "DeleteOnTermination": True,
        "VolumeType": "gp2",
        'VolumeSize': 100
        }
    }]


def create_instance(instance_type):
    print("Launching EC2 ...")
    ec2_resource = boto3.resource('ec2')
    # tag_specification = [{'ResourceType': 'spot-instances-request'}, ]
    instance_market_options={
	'MarketType': 'spot',
	'SpotOptions': {
	    'MaxPrice': '0.27',
	    'SpotInstanceType': 'one-time',
	}
    }

    instances = ec2_resource.create_instances(ImageId='ami-0c3fd0f5d33134a76',
	                                      MaxCount=1, MinCount=1,
	                                      InstanceType=instance_type,
	                                      IamInstanceProfile={'Name': 'SSM_Access'},
	                                      InstanceMarketOptions=instance_market_options,
                                              # BlockDeviceMappings=blockDeviceMappings,
    )
    time.sleep(5.0)


    instance = instances[0]
    return instance


def execute_command_list(instance_id, command_list):
    print("Executing {} ...".format(command_list))
    cmd_id_list = []
    cmd_id = execute_command(instance_id, command_list)
    cmd_id_list.append(cmd_id)
    print("cmd_id_list = {}".format(cmd_id_list))
    join_command_id(instance_id, cmd_id_list)


def execute_command(instance_id, command_list):
    ssm_client = boto3.client('ssm')
    try:
        response = ssm_client.send_command(
        DocumentName="AWS-RunShellScript",
            Parameters={'commands': command_list},
            InstanceIds=[instance_id],
            TimeoutSeconds=3600,
        )
    except Exception as e:
        print("Error: During Executing EC2 Instance")
        # print(dir(e))
        print("message:{0}".format(e.message))
        instance.terminate()
        exit()

    time.sleep(1.0)
    return response['Command']['CommandId']

def join_command_id(instance_id, cmd_id_list):
    ssm_client = boto3.client('ssm')

    cmd_finished = {}
    for cmd_id in cmd_id_list:
        cmd_finished[cmd_id] = False

    while True:
        all_cmd_done = True
        for cmd_id in cmd_id_list:
            if cmd_finished[cmd_id] == True:
                # When command is already finished, skipping show status
                continue

            output = ssm_client.get_command_invocation(
                CommandId=cmd_id,
                InstanceId=instance_id,
            )
            print("cmd_id = {}, Status = {}".format(cmd_id, output['Status']))

            is_cmd_finished = output['Status'] != 'InProgress'
            all_cmd_done = all_cmd_done & is_cmd_finished
            cmd_finished[cmd_id] = is_cmd_finished

            if cmd_finished[cmd_id] == True:
                print("Output = \n{}".format(output['StandardOutputContent']))
                print("Error  = \n{}".format(output['StandardErrorContent' ]))


        print("All Command Result = {}, Each Result = {}".format(all_cmd_done, cmd_finished))
        if all_cmd_done == True:
            break
        time.sleep(5.0)


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='EC2 LLVM Build Launcher.')
    parser.add_argument('instance_type', type=str,
                        help='Instance Type for Launch')
    parser.add_argument('--keep', dest='keep_instance', action='store_true',
                        help='Keep instance without shutdown')

    args = parser.parse_args()
    instance_type = args.instance_type
    keep_instance = args.keep_instance

    if instance_type == 't2.micro':
        job_num = '-j1'
    elif instance_type == 'c5.4xlarge':
        job_num = '-j16'
    else:
        print("Instance Type neither t2.micronor c5.4xlarge is not accepted.")
        exit()

    print("----------------------")
    print("Instance Type = {}".format(instance_type))
    print("Keep Instance = {}".format(keep_instance))
    print("----------------------")

    instance_id = ''
    instance = 0

    instance = create_instance(instance_type)

    print("Instance ID = {}".format(instance.instance_id))

    print("Waiting EC2 Launch ...")
    instance.wait_until_running()
    print("EC2 Launch Finished ...")

    instance.attach_volume(
        Device = '/dev/xvdb',
        InstanceId = instance.instance_id,
        VolumeId = 'vol-02c1e681a565eb322',
        )

    ec2_client = boto3.client('ec2')
    instance_statuses = ec2_client.describe_instance_status(InstanceIds=[instance_id])['InstanceStatuses']

    print("Instance Status = {}".format(instance_statuses[0]['InstanceStatus']['Details'][0]['Status']))

    while instance_statuses[0]['InstanceStatus']['Details'][0]['Status'] == 'initializing':
        time.sleep(5.0)
        instance_statuses = ec2_client.describe_instance_status(InstanceIds=[instance_id])['InstanceStatuses']
        print("Instance Status = {}".format(instance_statuses[0]['InstanceStatus']['Details'][0]['Status']))


    cmake_install_command_list = ['yum install -y gcc gcc-c++ ncurses-devel',
                                  'wget https://cmake.org/files/v3.10/cmake-3.10.0.tar.gz -O /tmp/cmake-3.10.0.tar.gz &&'
                                  'cd /tmp/ && tar xfz cmake-3.10.0.tar.gz &&'
                                  'cd /tmp/cmake-3.10.0 && ./bootstrap &> log &&'
                                  'make ' + job_num + ' -C /tmp/cmake-3.10.0/ &&'
                                  'make ' + job_num + ' install -C /tmp/cmake-3.10.0/'
    ]
    execute_command_list(instance_id, cmake_install_command_list)

    command_list = ['yum install update',
                    'yum install -y clang',
                    # 'aws s3 sync s3://llvm-bucket/build-myriscvx /home/ec2-user/build-myriscvx',
                    'aws s3 sync s3://llvm-bucket/llvm-myriscvx /home/ec2-user/llvm-myriscvx',
                    'aws s3 sync s3://llvm-bucket/myriscvx-tests /home/ec2-user/myriscvx-tests',
    ]
    execute_command_list(instance_id, command_list)

    command_list = ['cd /home/ec2-user/build-myriscvx && make ' + job_num]
    execute_command_list(instance.instance_id, command_list)

    command_list = ['aws s3 sync /home/ec2-user/build-myriscvx s3://llvm-bucket/build-myriscvx']
    execute_command_list(instance_id, command_list)

    if instance_keep == False:
        instance.terminate()
        print("Waiting EC2 Terminate ...")
        # instance.wait_until_terminated()
        print("EC2 Terminate Finished ...")
    else:
        print("Instance Type {} is keeped. Don't forget shutting down.".format(instance_id))
