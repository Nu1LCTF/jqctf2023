# webshell-ng

出个题的想法就是：在不用eval的情况下，实现webshell的所有功能。

## 解题

1. 利用phpinfo，获得php版本(8.2.12)，disable function列表和open_basedir

2. 利用scandir可以扫描web目录，发现config.php，里面有数据库帐号密码

3. 连接数据库获得前半段flag和一个提示（要检查根目录）

4. 利用file_put_contents上传一个so，再用fsockopen链接到fpm，加载上传的so完成RCE

## 坑点

1. 需要利用反射完成多个参数函数的调用

2. 有一些师傅尝试绕open_basedir，目前在8.2版本里，传统的绕过方法基本上都失效了

3. 有一些师傅直接人脑生成指令没有写脚本，导致后面需要打fpm的时候思考失败

给掉在坑里的师傅道个歉QAQ