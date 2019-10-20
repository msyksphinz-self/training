#!/usr/bin/python3

import time
import boto3

blockDeviceMappings = [{
    "DeviceName": "/dev/xvda",
    "Ebs": {
        "DeleteOnTermination": True,
        "VolumeType": "gp2",
        'VolumeSize': 100
        }
    }]

def create_instance(instance_type):
    print("Launching EC2 ...")
    ec2_resource = boto3.resource('ec2')
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
                                              BlockDeviceMappings=blockDeviceMappings,
    )
    time.sleep(5.0)


    instance = instances[0]
    return instance


def execute_command_list(instance_id, command_list):
    print("Executing {} ...".format(command_list))
    cmd_output = execute_command(instance.instance_id, command_list)
    print("Output = \n{}".format(cmd_output['StandardOutputContent']))
    print("Error  = \n{}".format(cmd_output['StandardErrorContent']))


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
        print("message:{0}".format(e.message))
        instance.terminate()
        exit()

    time.sleep(5.0)
    time_cnt = 0

    command_id = response['Command']['CommandId']
    output = ssm_client.get_command_invocation(
        CommandId=command_id,
        InstanceId=instance_id,
    )
    while(output['Status'] == 'InProgress'):
        print("Time = {} : Status = {}".format(time_cnt * 5.0, output['Status']))

        time.sleep(5.0)
        output = ssm_client.get_command_invocation(
            CommandId=command_id,
            InstanceId=instance_id,
        )
        time_cnt += 1

    return output

instance = create_instance('t2.micro')
# instance = create_instance('c5.4xlarge')
print("Instance ID = {}".format(instance.instance_id))


print("Waiting EC2 Launch ...")
instance.wait_until_running()
print("EC2 Launch Finished ...")

ec2_client = boto3.client('ec2')
instance_statuses = ec2_client.describe_instance_status(InstanceIds=[instance.instance_id])['InstanceStatuses']

print("Instance Status = {}".format(instance_statuses[0]['InstanceStatus']['Details'][0]['Status']))

while instance_statuses[0]['InstanceStatus']['Details'][0]['Status'] == 'initializing':
    time.sleep(5.0)
    instance_statuses = ec2_client.describe_instance_status(InstanceIds=[instance.instance_id])['InstanceStatuses']
    print("Instance Status = {}".format(instance_statuses[0]['InstanceStatus']['Details'][0]['Status']))


# cmake_install_command_list = ['yum install -y gcc gcc-c++ ncurses-devel',
#                               'wget https://cmake.org/files/v3.10/cmake-3.10.0.tar.gz -O /tmp/cmake-3.10.0.tar.gz',
#                               'cd /tmp/ && tar xfz cmake-3.10.0.tar.gz',
#                               'cd /tmp/cmake-3.10.0 && ./bootstrap &> log',
#                               'make -j16 -C /tmp/cmake-3.10.0/',
#                               'make -j16 install -C /tmp/cmake-3.10.0/',
#                               'whereis cmake',
# ]
# execute_command_list(instance.instance_id, cmake_install_command_list)

command_list = ['ls -lt /',
                'df -h',
]
execute_command_list(instance.instance_id, command_list)

instance.terminate()
print("Waiting EC2 Terminate ...")
# instance.wait_until_terminated()
print("EC2 Terminate Finished ...")
