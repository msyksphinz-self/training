#!/usr/bin/env python
# -*- coding: utf-8 -*-

import requests
import bs4
import re
import time
import matplotlib.pyplot as plt
# from wordcloud import WordCloud
from bs4 import BeautifulSoup
import requests
import MeCab as mc
from wordcloud import WordCloud

hatena_id = "msyksphinz"
blog_id = "msyksphinz.hatenablog.com"
password = "xxx"

def get_collection_uri(hatena_id, blog_id, password):
    service_doc_uri = "https://blog.hatena.ne.jp/{hatena_id:}/{blog_id:}/atom".format(hatena_id=hatena_id, blog_id=blog_id)
    res_service_doc = requests.get(url=service_doc_uri, auth=(hatena_id, password))
    if res_service_doc.ok:
        soup_servicedoc_xml = bs4.BeautifulSoup(res_service_doc.content, features="xml")
        collection_uri = soup_servicedoc_xml.collection.get("href")
        return collection_uri

    return False

def mecab_analysis(text):
    t = mc.Tagger('-Ochasen -d /usr/lib/x86_64-linux-gnu/mecab/dic/mecab-ipadic-neologd/')
    enc_text = text.encode('utf-8')
    node = t.parseToNode(enc_text)
    output = []
    while(node):
        if node.surface != "":  # ヘッダとフッタを除外
            word_type = node.feature.split(",")[0]
            if word_type in ["形容詞", "動詞","名詞", "副詞"]:
                output.append(node.surface)
        node = node.next
        if node is None:
            break
    return output


def wordcloud(text):
    # wordcloud = WordCloud(background_color="white", font_path="/System/Library/Fonts/ヒラギノ明朝 ProN W3.otf",width=1024,height=674).generate(text)
    wordcloud = WordCloud(background_color="white",
                          font_path="/usr/share/fonts/truetype/vlgothic/VL-PGothic-Regular.ttf",
                          width=1024,
                          height=674).generate(text)
    wordcloud.to_file("./test1.png")


collection_uri = get_collection_uri(hatena_id, blog_id, password)

text = ""

MAX_ITERATER_NUM = 50
for i in range(MAX_ITERATER_NUM):
    # Basic認証で記事一覧を取得
    res_collection = requests.get(collection_uri, auth=(hatena_id, password))
    if not res_collection.ok:
        print("faild")
        continue
    # Beatifulsoup4でDOM化
    soup_collectino_xml = bs4.BeautifulSoup(res_collection.content, features="xml")
    # entry elementのlistを取得
    entries = soup_collectino_xml.find_all("entry")
    # 下書きを無視
    pub_entry_list = list(filter(lambda e: e.find("app:draft").string != "yes", entries))
    for e in pub_entry_list:
        text += e.title.string.encode('utf-8')

    # next
    link_next = soup_collectino_xml.find("link", rel="next")
    if not link_next:
        break
    collection_uri = link_next.get("href")
    if not collection_uri:
        break
    time.sleep(0.01)# 10ms

mecab_wordlist = mecab_analysis(text.decode('utf-8'))
mecab_wordlist = filter(lambda x: unicode(x, 'utf-8') != unicode('みる', 'utf-8'), mecab_wordlist)
mecab_wordlist = filter(lambda x: unicode(x, 'utf-8') != unicode('やっ', 'utf-8'), mecab_wordlist)
mecab_wordlist = filter(lambda x: unicode(x, 'utf-8') != unicode('する', 'utf-8'), mecab_wordlist)

for word in mecab_wordlist:
    print word
    # print word.encode('utf-8')

wordcloud(" ".join(mecab_wordlist).decode('utf-8'))
# wordcloud(text.decode('utf-8'))
