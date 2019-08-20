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
    print("Launching EC2..")
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
                                              BlockDeviceMappings=blockDeviceMappings,
    )
    time.sleep(5.0)


    instance = instances[0]
    return instance

def execute_command(instance_id, command):
    ssm_client = boto3.client('ssm')
    try:
        response = ssm_client.send_command(
        DocumentName="AWS-RunShellScript",
            Parameters={'commands': [command]},
            InstanceIds=[instance_id],
        )
    except Exception as e:
        print("Error: During Executing EC2 Instance")
        # print(dir(e))
        print("message:{0}".format(e.message))
        instance.terminate()
        exit()

    time.sleep(5.0)

    command_id = response['Command']['CommandId']
    output = ssm_client.get_command_invocation(
        CommandId=command_id,
        InstanceId=instance_id,
    )

    return output

instance = create_instance('t2.micro')
print("Instance ID = {}".format(instance.instance_id))


print("Waiting EC2 Launch ...")
instance.wait_until_running()
print("EC2 Launch Finished ...")

ec2_client = boto3.client('ec2')
instance_statuses = ec2_client.describe_instance_status(InstanceIds=[instance.instance_id])['InstanceStatuses']
print(instance_statuses)

while instance_statuses[0]['InstanceStatus']['Details'][0]['Status'] == 'initializing':
    time.sleep(5.0)
    instance_statuses = ec2_client.describe_instance_status(InstanceIds=[instance.instance_id])['InstanceStatuses']
    print(instance_statuses)


# command_list = ['sudo yum install -y gcc',
#                 'ls -lt /',
#                 'echo #include <stdio.h>\n \
# int main() { \
#   printf(\"Hello World\"); \
#   return 0; \
# } > main.c',
#                 'cat main.c',
#                 'gcc main.c -o main',
#                 './main']

# command_list = ['ls -lt /',
#                 'df -h',
#                 ]

command_list = ['aws s3 help',
                'pwd',
                'df -h',
                'ls -lt /home/',
                'aws s3 sync s3://llvm-bucket/llvm-myriscvx/ /home/ec2-user/ ',
                'aws s3 sync s3://llvm-bucket/myriscvx-tests/ /home/ec2-user/ ',
]

for command in command_list:
    cmd_output = execute_command(instance.instance_id, command)
    print("Output = \n{}\n".format(cmd_output['StandardOutputContent']))
    print("Error  = \n{}\n".format(cmd_output['StandardErrorContent']))


instance.terminate()

print("Waiting EC2 Terminate ...")
# instance.wait_until_terminated()
print("EC2 Terminate Finished ...")
