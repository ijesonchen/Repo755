# UbuntuCloud

- - -

第五次安装
恢复镜像8
MAAS正常
stack安装完成
生成sshkey
enlist

MAAS设置cluster管理DHCP和DNS，否则无法解析node名


sudo JUJU_BOOTSTRAP_TO=vmmaas1604ap.maas openstack-install
sudo: devstack-install: command not found

？？？？

- - -
第四次
接第三次快照

## 配置固定IP

## maas DHCP配置网关

## 更换163源

## git源
安装git
复制git库（预先签出)
~/devstack
/opt/stack/xxx

## 下载
http://download.cirros-cloud.net/0.3.4/cirros-0.3.4-x86_64-uec.tar.gz
放入
devstack/files

## stack user
cd ~/devstack
sudo ./tools/create-stack-user.sh
sudo su
echo "stack ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
exit

## ./stack.sh
=========================
DevStack Component Timing
=========================
Total runtime         3300

run_process            57
test_with_retry         4
apt-get-update         14
pip_install           1575
restart_apache_server  15
wait_for_service       15
apt-get               472
=========================
This is your host IP address: 192.168.13.80
This is your host IPv6 address: ::1
Horizon is now available at http://192.168.13.80/dashboard
Keystone is serving at http://192.168.13.80/identity/
The default users are: admin and demo
The password: 111111

## 添加sshkey
ssh-keygen -t rsa -b 4096 -C "cjx@ulang.com"
快照8
maas正常，openstack安装完成
关机 快照9

## openstack安装后MAAS无法访问
尝试修复，否则恢复至快照8

dpkg-reconfigure maas-cluster-controller
提示
maas-clusterd stop/waiting
Traceback (most recent call last):
  File "/usr/sbin/maas-provision", line 55, in <module>
    main()
  File "/usr/sbin/maas-provision", line 51, in main
    run()
  File "/usr/sbin/maas-provision", line 43, in run
    from provisioningserver.__main__ import main
  File "/usr/lib/python2.7/dist-packages/provisioningserver/__main__.py", line 18, in <module>
    import provisioningserver.boot.install_bootloader
  File "/usr/lib/python2.7/dist-packages/provisioningserver/boot/__init__.py", line 29, in <module>
    from provisioningserver.boot.tftppath import compose_image_path
  File "/usr/lib/python2.7/dist-packages/provisioningserver/boot/tftppath.py", line 26, in <module>
    from provisioningserver.drivers.osystem import (
  File "/usr/lib/python2.7/dist-packages/provisioningserver/drivers/__init__.py", line 27, in <module>
    from provisioningserver.power.schema import JSON_POWER_TYPE_PARAMETERS
  File "/usr/lib/python2.7/dist-packages/provisioningserver/power/__init__.py", line 23, in <module>
    from provisioningserver.rpc.region import UpdateNodePowerState
  File "/usr/lib/python2.7/dist-packages/provisioningserver/rpc/region.py", line 43, in <module>
    from provisioningserver.rpc.arguments import (
  File "/usr/lib/python2.7/dist-packages/provisioningserver/rpc/arguments.py", line 27, in <module>
    from apiclient.utils import ascii_url
ImportError: No module named utils

尝试pip安装urlib3 utils apiclient urlparser2/3/4 未解决。

恢复镜像8



- - -

# 第三次
第二次安装恢复快照至系统安装完成

## install add-apt-repository
sudo apt-get install software-properties-common python-software-properties

## cluster manage
sudo maas-region-admin createadmin
修改/etc/apache2/apache2.conf添加
ServerName 192.168.13.210
dpkg-reconfigure maas-cluster-controller
dpkg-reconfigure maas-region-controller
reboot

## import image

## install pyvmomi

## install openstack
add cloud repo
apt install openstack
失败 Hash Sum mismatch
functions-common:apt_get_update die 1075 'Failed to update apt repos, we're died'

### 更换163源（删除原来source.list，用163源覆盖）

deb http://mirrors.163.com/ubuntu/ trusty main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ trusty-security main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ trusty-updates main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ trusty-proposed main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ trusty-backports main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ trusty main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ trusty-security main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ trusty-updates main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ trusty-proposed main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ trusty-backports main restricted universe multiverse


### 配置内外网固定IP
自动设置路由脚本
路由IP分配有问题

### 提前签出git库（11个）
使用devstack安装
过称过慢，可以先使用git将需要的东西迁出
在windows下签出时，注意和linux换行符不同可能导致脚本运行错误
1. /home/user/devstack
2. /opt/stack:
10个
git clone git://git.openstack.org/openstack/cinder.git --branch master
git clone git://git.openstack.org/openstack/horizon.git --branch master
git clone git://git.openstack.org/openstack/glance.git --branch master
git clone git://git.openstack.org/openstack/keystone.git --branch master
git clone git://git.openstack.org/openstack/neutron.git --branch master
git clone git://git.openstack.org/openstack/nova.git --branch master
git clone https://github.com/kanaka/noVNC.git --branch master
git clone git://git.openstack.org/openstack/requirements.git --branch master
git clone git://git.openstack.org/openstack/swift.git --branch master
git clone git://git.openstack.org/openstack/tempest.git --branch master

** 快照6 准备安装openstack **

## 创建stack用户
cd ~/devstack
sudo tools/create-stack-user.sh
sudo su
echo "stack ALL=(ALL) NOPASSWD: ALL" >> /etc/sudoers
exit
## ~/devstack



- - -

修改默认路由
sudo ip route del default
sudo ip route add default via 192.168.13.1 dev eth0

- - -



# Local Image Mirror
主机 vmmaas1604rec01
http://maas.io/docs/en/installconfig-images-mirror


sudo apt install simplestreams

KEYRING_FILE=/usr/share/keyrings/ubuntu-cloudimage-keyring.gpg
IMAGE_SRC=https://images.maas.io/ephemeral-v2/daily/
IMAGE_DIR=/var/www/html/maas/images/ephemeral-v2/daily

sudo sstream-mirror --keyring=$KEYRING_FILE $IMAGE_SRC $IMAGE_DIR 'arch=amd64' 'release~(trusty|xenial)' --max=1 --progress



The images will be written to disk in the directory defined by the variable 'IMAGE_DIR' above and the 'location' of the new boot source will be:

URL=http://<myserver>/maas/images/ephemeral-v2/daily/



- - -

# 第二次安装
参考
https://www.ubuntu.com/download/cloud
http://docs.openstack.org/developer/devstack/#create-a-local-conf

## 新建虚拟机，双网卡，网卡1 外网 网卡2 仅主机

## 安装ubuntu with maas 14.04
外网DHCP配置网卡1 固定ip 192.168.13.80

## 配置MAAS

未执行 sudo apt-get install maas
执行 sudo maas-region-admin createadmin

## 登录UI，sync boot-images
没有下载进度，推测需要一天时间

## cluster未连接
安装时获取到的IP和后来DHCP 分配到网卡1的IP不一样
修改/etc/apache2/apache2.conf添加ServerName
dpkg-reconfigure maas-cluster-controller
dpkg-reconfigure maas-region-controller
配置为正确的IP地址

## 安装openstack

安装完成后没有执行
sudo add-apt-repository ppa:cloud-installer/stable
sudo apt-get update
而是参考dev-stack的安装
在user账号下执行stack.sh
脚本执行时间较长，需要更新几个比较大的包，至少需要1个小时。
 stack.sh completed in 10210 seconds

- - -



# 环境
> VmWare 11.0, 4 core, 4G, 200G
> ubuntu 14.04.2 server
> 基于openstack

# 步骤
## 新建虚拟机1，安装ubuntu with MAAS
按照 [OpenStack Autopilot](http://https://www.ubuntu.com/download/cloud)说明进行安装
> 新建虚拟机，添加两个虚拟网卡，一个虚拟网卡用来和外界通信，另一个虚拟网卡用来内部部署。内网卡用于MAAS部署DHCP服务
> 安装ubuntu with maas， 中途安装grub时断开网络导致错误，重试即可
> 语言选中文时，16.04server终端会显示乱码，装中文资源和zhcon未解决。选择英文
> sudo: add-apt-repository: command not found
sudo apt-get install software-properties-common python-software-properties
> http://[ip]/MAAS 其中MAAS一定要大写

## MAAS中导入boot images
大约需要20小时左右。导入完成后image 会显示synced

## 配置内网ip地址
MAAS服务器需要配置内网静态地址，并启用DHCP服务
ip addr 查看ip地址并记下 接口名称，从是否有IP地址可以区分两个网卡

备份
sudo cp /etc/network/interfaces /etc/network/interfaces.bak
修改配置
sudo vim /etc/network/interfaces

修改如下部分，将eth0改为内网接口名称

auto eth0
iface eth0 inet static
address 192.168.0.117
gateway 192.168.0.1
netmask 255.255.255.0

注意：系统已最后一个接口为默认网关（路由）

重启服务
service network restart
或
service networking restart

MASS 1.9（ubuntu 14.04）需要配置DHCP为启动（详细步骤）

## 其他机器选择PXE启动
## MAAS管理界面，commsion相应机器
power type为vmware时，需要安装python-pyvmomi管理vmware虚拟机
pyvmomi版本必须为5.5.0
已证实6.5.0无法power on / off vm虚拟机




注意配置eth1 ip gw后可能导致默认路由改变，无法访问外网.修改默认路由
sudo ip route del default
sudo ip route add default via 192.168.13.1 dev eth0
可将上述脚本添加到
/etc/rc.local
中，自动执行

## pyvmomi

Power control software is missing from the cluster controller. To proceed, install the python-pyvmomi package on the Cluster master cluster.

添加ppa后，sudo apt install  python-pyvmomi找不到安装源
参考 
https://bugs.launchpad.net/maas/+bug/1499934
考虑使用pip安装该包
安装sudo apt install python-pip
pip install --upgrade pyvmomi --fix-missing ？？？
安装版本6.5
添加PPA
http://blog.csdn.net/zmjingying/article/details/7110109

查看安装包版本
pip freeze|grep pyvmomi
pip uninstall pyvmomi
pip install pyvmomi==5.5.0

## pyvmomi示例代码
获取虚拟机列表
http://xujunxian.blog.51cto.com/8614409/1742101
启动虚拟机
https://github.com/vmware/pyvmomi/blob/master/sample/poweronvm.py

windows下python2.7
py poweronvm.py -s 192.168.13.240 -o 444 -u administrator -p ulang51159270 -v [VmMachineName]


## pyvmomi兼容性
https://pypi.python.org/pypi/pyvmomi

Compatibility Policy

pyVmomi versions are marked vSphere_version-release . Pyvmomi maintains minimum backward compatibility with the previous _four_ releases of vSphere and it’s own previous four releases. Compatibility with much older versions may continue to work but will not be actively supported.

For example, version v6.0.0 is most compatible with vSphere 6.0, 5.5, 5.1 and 5.0. Initial releases compatible with a version of vSphere will bare a naked version number of v6.0.0 indicating that version of pyVmomi was released simultaneously with the GA version of vSphere with the same version number.

## 查看maaslog
1. tail -f /var/log/maas/maas.log
2. UI界面最下端可以看到相关信息，如node操作界面

## 更改外网IP后，clustet无法连接
One cluster is not yet connected to the region. Visit the clusters page for more information.
删除了cluster0后，clusters中看不到看不到cluster0
http://askubuntu.com/questions/580784/connect-cluster-controller-to-the-region-controller-maas

http://askubuntu.com/questions/748142/one-cluster-is-not-yet-connected-to-the-region-after-install-ubuntu-server
sudo dpkg-reconfigure maas-cluster-controller
可以看到cluster master
sudo dpkg-reconfigure maas-region-controller
输入IP地址

## maas-region-controller Could not reliably determine the server's fully qualified domain name
user@vmu1404mass-tmpl:~$ sudo dpkg-reconfigure maas-region-controller

AH00558: apache2: Could not reliably determine the server's fully qualified domain name, using 127.0.1.1. Set the 'ServerName' directive globally to suppress this message

修改/etc/apache2/apache2.conf添加ServerName
chmod 777 /etc/apache2/apache2.conf
cp /etc/apache2/apache2.conf /etc/apache2/apache2.conf.bak
vi /etc/apache2/apache2.conf
追加
ServerName 192.168.13.210
重新运行sudo dpkg-reconfigure maas-region-controller即可

## maas-enlisting-node login
http://askubuntu.com/questions/173112/what-is-the-maas-node-login
http://askubuntu.com/questions/315647/maas-nodes-stuck-on-maas-enlisting-node

## Launch OpenStack Autopilot
E: Unable to locate package openstack

sudo add-apt-repository ppa:cloud-installer/stable
sudo apt update
sudo apt-get install openstack

> sudo apt update
Failed to fetch http://ppa.launchpad.net/cloud-installer/stable/ubuntu/dists/trusty/main/binary-amd64/Packages  Hash Sum mismatch

## 使用dev-stack安装openstack
同样的hash sum mismatch问题
16.04下apt install openstack正常


## ubuntu openstack版本命名：以单词按照首字母序递增排列。如ubuntu 14.04 T开头，14.10U开头，16.04x开头 


