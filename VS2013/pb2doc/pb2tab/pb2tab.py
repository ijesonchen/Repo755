
s = r'''

    optional OperationType   opertype   = 1; // 操作类型（增删）
    optional string userid   = 2; // 业务系统用户唯一标识
    optional string account  = 3; // 账户
    optional string password = 4; // 密码
    optional string spyid    = 5; // 业务系统ID


'''

a = s.split('\n')

for l in a:
    s2 = l.replace(';', ' ').replace('//', ' ').replace('=', ' ')
    a2 = s2.split()
    if len(a2) == 2:
        print()
        for s3 in a2:
            print(s3, end = ' ')
        print()
        print()
        print('限定符 类型 名字 ID 说明')
    if len(a2) >= 4:
        for s3 in a2:
            print(s3, end=' ')
        print()

