#在控制台中输入python指令： pip install 库的名字  
#就可以自动下载安装库，然后就可以导入了
import requests
import sys
import time

from fake_useragent import UserAgent

def DownloadFile(url, filepath):
    with open(filepath, "wb") as f:	#r:只读，r+：读写，w:清空源文件并只写，a:添加模式
        ua = UserAgent()
        headers = {"User_Agent": ua.chrome}	#headers为网址提供了一个头，就是放在  <>http://...  中<>位置的一个前缀
        response = requests.get(url, headers = headers, stream = True)	#想获取来自服务器的原始套接字响应就要设置stream = True
        total = response.headers.get("content-length")
        if total is None:
            f.write(response.content)
        else:
            downlode = 0
            total = int(total)
            startTime = time.time()	#记录开始时间
				#遍历读到的内容
            for data in response.iter_content(chunk_size = max(int(total / 1000), 1024*1024)):
                downlode += len(data)
                f.write(data)
                done = int(50 * downlode/total)
                percentage = (done / downlode) * 100
                elapsedTime = time.time() - startTime
                avgKBPerSecond = (downlode / 1024) / elapsedTime	#计算下载速度
                avgSpeedString = '{:.2} KB/s'.format(avgKBPerSecond)#显示下载速度
                #avgSpeedString = f'{avgKBPerSecond} KB/s'	//我觉得也可以的另一种的表达方式
                if (avgKBPerSecond > 1024):
                    avgKBPerSecond /= 1024	#不再显示KB，显示MB
                    avgSpeedString = '{:.2} MB/s'.format(avgKBPerSecond)
                sys.stdout.write('\r[{}{}] {:.2f}% ({})     '.format('█' * done, '.' * (50-done), percentage, avgSpeedString))	# ███████......
                sys.stdout.flush()	#清空刷新
    sys.stdout.write('\\n')
            
def YesOrNo():
    while True:
        reply = str(input("[Y/N]: ")).lower().strip()
        if(reply[0 : 1] == "y"):
            return True
        elif(reply[0 : 1] == "n"):
            return False
        else:
            sys.stdout.write('ERROR: Please input Y(YES) or N(No)')
        

            