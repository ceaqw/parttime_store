from selenium import webdriver
import requests
import time

def msg_deal(st_second):
    """信息流处理函数"""
    url = "http://looktop.cn/mao/"
    post_url = "http://looktop.cn/post/?id=123&html"

    ua = "user-agent=Mozilla/5.0 (Windows NT 10.0; WOW64; Trident/7.0; rv:11.0) like Gecko"

    options = webdriver.ChromeOptions()
    options.add_argument(ua)
    # options.add_argument("--headless")

    driver = webdriver.Chrome(options=options)

    while True: 
        print("刷新一次，时间------->",time.ctime())

        driver.get(url)
        msg_get = driver.page_source.split("<body>")[-1].split("</body>")[0]
        msg_token_list = msg_get.split("|")

        print(msg_token_list)

        if msg_token_list[1] == 'look':
            driver.get(msg_token_list[2])
            time.sleep(0.2)

            data = {"id":msg_token_list[0],"html":driver.page_source}

            requests.post(post_url,data)    

            print("数据包data----->",data)  
        else:
            pass
        time.sleep(st_second)
    
def main():
    # st_second = int(input("输入刷新时间间隔(单位s):"))
    st_second = 5
    msg_deal(st_second)

if __name__ == "__main__":
    main()