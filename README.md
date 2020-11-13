# echo-client-server    

echo-client:    
syntax : echo-client <ip> <port>    
sample : echo-client 192.168.10.2 1234    
    
echo-server:    
syntax : echo-server <port> [-e[-b]]    
sample : echo-server 1234    
sample : echo-server 1234 -e    
sample : echo-server 1234 -e -b    

# example 1    
    
make    
./echo-server 2212     
./echo-client 127.0.0.1 2212    

Client 측    
    
처음으로 나오는 빈 창에 server로 보내고 싶은 메세지를 치면, 그것이 서버로 보내집니다. 

Client program begun    
Client program connection established    

RecvThread is working!    
abcdef    
RecvThread is working!    
12233    
    
Server 측    
    
Echo server program begun    
Connected and recvThread is working!    
message : abcdef    
message : 12233    
...    
    
    
# example 2    
    
make        
./echo-server 2212     
./echo-client 127.0.0.1 2212    
    
Client 측    
    
처음으로 나오는 빈 창에 server로 보내고 싶은 메세지를 치면, 그것이 서버로 보내져서 다시 돌아옵니다.  
    
Client program begun    
Client program connection established    
    
RecvThread is working!    
22212    
message sent from server : 22212    
3123213311    
message sent from server : 3123213311    
^C
    
Server 측
    
Client한테 받은 메세지를 그대로 돌려줍니다.    
    
Echo server program begun    
Connected and recvThread is working!    
message : 22212    
message : 3123213311    
recv failed: Success    
    
client가 연결을 끊었으면, "recv failed: Success"
라는 문구를 출력해 줍니다.    
    
# example 3
make        
./echo-server 2212          
./echo-client 127.0.0.1 2212    
./echo-client 127.0.0.1 2212    
./echo-client 127.0.0.1 2212    
    
이 상태에서 server는 client에게 받은 메세지를 broadcast해 줍니다.     
아래에서 볼 수 있듯이, 연결이 성립되었으면 "Connected and recvThread is working!"문구를,     
메세지를 client가 보냈으면 그것을 broadcast로 몇 명의 client에게 보내는지를 출력해 줍니다.    
    
Echo server program begun    
Connected and recvThread is working!    
message : dsdas    
broadcast send to 1 number of clients    
message : dassadasad    
broadcast send to 1 number of clients    
Connected and recvThread is working!    
message : adsadsadsdad    
broadcast send to 2 number of clients    
message : addaadsasd    
broadcast send to 2 number of clients    
Connected and recvThread is working!    
message : adsdasads    
broadcast send to 3 number of clients    
message : adsasdsadasdads    
broadcast send to 3 number of clients    
...
