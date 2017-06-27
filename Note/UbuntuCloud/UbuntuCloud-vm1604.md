# UbuntuCloud

# 环境
> host: Dell R730, E5-2640V3*2 2.6G（8core*2 + HT)
> os: windows server 2008 r2 ent.
> network: 192.168.13.240, mask 255.255.255.0, gateway 192.168.13.1, dns 192.168.13.1
> VmWare workstation pro 12.5, 4 core, 4G, 200G
> ubuntu 16.04.1 LTS server amd64
> openstack

# 步骤
参考 [OpenStack Autopilot](https://www.ubuntu.com/download/cloud)说明进行安装

## vmware虚拟网络编辑器
VMNet0:桥接模式，如果主机有多个网卡，桥接到正确的网卡
VMNet8：
	NAT模式，修改子网IP为相应网段(192.168.200.0/255.255.255.0)
    取消 使用本地DHCP服务器将IP地址分配给虚拟机 （MAAS DHCP负责）
    NAT设置,网关IP： 192.168.200.254(网段受子网IP限制)
    一开始建议NAT连通网络，容易导致enlist机器时通过网络更新数据，速度过慢。


## 安装MAAS controler
新建虚拟机 vmmaas1604ctrl
网卡1：VMnet0
网卡2：VMnet8
设置，选项，高级，勾选EFI引导
安装系统，选择install maas region control，安装语言选择英文.
primary network interface:一般选择第一个，如果后面DHCP失败可以返回修改。
设置maas账号和密码（MAAS UI管理页面用）
设置ubuntu账号密码
不加密home目录

分区选entire disk and setup LVM, force UEFI installation



安装时选择中文的可能导致的问题：
	1. 字符界面显示乱码，安装zhcon未解决。
	2. 启动方式选择BIOS时会导致安装失败，提示某个包错误。
使用EFI模式启动可能导致的问题：启动后虚拟机界面黑屏，但是可以远程登录，MAAS服务也正常。

## 配置MAAS UI
生成SSH key
ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
默认key存储路径  home/[username]/.ssh/id_rsa.pub
登录 http://[ip]/MAAS
其中MAAS一定要大写
选择自动部署的ubuntu版本，SSH key选upload，将id_rsa.pub中的内容粘贴进去。

## 配置内网ip地址
MAAS服务器需要配置内网静态地址，并启用DHCP服务
ip addr 查看ip地址并记下 接口名称ens34，从是否有IP地址可以区分两个网卡

备份
sudo cp /etc/network/interfaces /etc/network/interfaces.bak
修改配置
sudo vim /etc/network/interfaces

添加如下部分

# maas dhcp interface
auto ens34
iface ens34 inet static
address 192.168.200.1
gateway 192.168.200.254
netmask 255.255.255.0

重启网络
service networking restart

注意：添加ip配置后，可能导致默认路由的改变。
route查看默认路由，不对就调整ens33 ens34的顺序



登录到UI，nodes， controller，进入控制器vmmaas1604ctrl.maas
Interfaces:ens34对应的fabric-35
Served VLANs，fabric-35，VLAN，进入untagged
右上角，tack action, provide dhcp
重启系统
登录到UI，nodes， controller，进入控制器vmmaas1604ctrl.maas
检查 Services 中dhcpd已启动

管理虚拟机VMWARE
apt install python-pyvmomi --fix-missing

Rack Controller下
rackd未运行，提示rackd — Missing connections to 1 region controller(s).
运行dpkg-reconfigure maas-region-controller即可

enlist时，机器无法连接外网的话，会导致无法enlist，下载一些东西下载不到，最后停留在登陆界面
正常应该是enlist之后自动关闭


- - -

# 问题：node power type无法正确设置
相同信息，在14.04上面可以正常check power，开关机（vm版本11.0/12.5通过），但是16.04不行。错误提示


Failed to query node's BMC - (root) - No rack controllers can access the BMC of node: vmmaas1604c05
在机器启动时，task action选择commsing
错误提示
Marking node failed - Power on for the node failed: Failed talking to node's BMC: (vim.fault.HostConnectFault) { dynamicType = <unset>, dynamicProperty = (vmodl.DynamicProperty) [], msg = '[SSL: CERTIFICATE_VERIFY_FAILED] certificate verify failed (_ssl.c:645)', faultCause = <unset>, faultMessage = (vmodl.LocalizableMessage) [] }

Node changed status - From 'Commissioning' to 'Failed commissioning'

Failed to power on node - Power on for the node failed: Failed talking to node's BMC: (vim.fault.HostConnectFault) { dynamicType = <unset>, dynamicProperty = (vmodl.DynamicProperty) [], msg = '[SSL: CERTIFICATE_VERIFY_FAILED] certificate verify failed (_ssl.c:645)', faultCause = <unset>, faultMessage = (vmodl.LocalizableMessage) [] }

Failed to query node's BMC - __init__() takes 1 positional argument but 2 were given

PXE Request - power off

pyvmomi版本都是5.5.0


更换pyvmomi版本为6.5.0，仍然提示
Failed to query node's BMC - (admin) - No rack controllers can access the BMC of node: vmmaas1604c05

方案？
多装一个controller？

使用14.04？

换用pyvmomi 6.0？


- - -

power type设置为manual后，可以手动开机/关机commssing
但是node会提示类似
cloud-init : handlers.py[WARNING]: failed posting event: start: modules-final/config-final-message: SUCCCESS: config-final-message ran successfully
... failed posting event ... config-power-state-change ran successfully

python脚本bug？pyvmomi版本不对？曾经发生过14.04使用pyvmomi 6.5无法power node。更换为5.5正常
但是使用官方教程apt get python-pyvmomi安装的版本就是5.5.0。更换为6.5.0也不行



通信问题？
ufw status
service iptables status
都是inactive

路由问题？
应该是存取13.240 vmware服务器问题，但是能正常上网，能ping 240



- - -


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
py py2power.py -s 192.168.13.240 -o 444 -u administrator -p ulang51159270 -a on -n 

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


