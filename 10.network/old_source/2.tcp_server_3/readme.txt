tcp-server

程序名称tcp-server
直接运行 端口号为60000
如果需要修改端口号 使用参数传入

1、能够接收多个客户端的连接
2、每当一个新的客户端连接时，显示 ==== client_id ipv4 / port connected    ====
3、每当一个客户端断开连接时，显示 #### client_id ipv4 / port disconnected ####
4、当已经连接的客户端发送消息时，能够接收消息并输出在终端
5、要给客户端发送消息，输入/socket_id:要发送给目标客户端的信息
   输入 /list	可以查看目前所有连接到服务端的客户端信息
	/shutdown 6


4 -> getpeername()

一个线程处理多个客户端数据 多路IO复用 - select()


