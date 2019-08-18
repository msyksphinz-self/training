#!/usr/bin/python3

import time
import boto3

instance_id = 'i-0437c8e0c7dd105dc'

ssm_client = boto3.client('ssm', region_name='ap-northeast-1')
result = ssm_client.send_command(
    InstanceIds=[instance_id],
    DocumentName="AWS-RunShellScript",
    Parameters={'commands': ['ls -lt /']},
)

time.sleep(5.0)

command_id = result['Command']['CommandId']
print("Command ID = {}\n".format(command_id))
output = ssm_client.get_command_invocation(
    CommandId=command_id,
    InstanceId=instance_id,
)
print("Output = \n{}\n".format(output['StandardOutputContent']))
