#!/usr/bin/env python3
from google.cloud import translate_v2 as translate
import sys
import os
import re

SEPARATOR_META_DATA = "---"
SEPARATOR_CODE = "```"
SEPARATOR_IMG = "!["
SEPARATOR_IMG_CODE = "<img"
STR_TITLE = "Title:"
STR_URL = "URL:"
STR_EDITURL = "EditURL:"
STR_STAR = "*"
STR_SHARP = "#"
STR_DESCRIPTION = "description:"

translate_client = translate.Client()
USE_GOOGLE_TRANS = True

def is_english(text):
    for c in text:
        if not 0 <= ord(c) <= 128:
            return False
    return True


def correct_code_text(old, new):
    CHAR_CODE = '`'
    old_code_cnt = old.count(CHAR_CODE)
    new_code_cnt = new.count(CHAR_CODE)
    result = new
    if (old_code_cnt == new_code_cnt) and (old_code_cnt%2 == 0) and (new_code_cnt%2 == 0):
        old_idx = 0
        new_idx = 0
        for i in range(0, int(old_code_cnt/2)):
            first_old = old.index(CHAR_CODE, old_idx)
            old_idx = first_old + 1
            second_old = old.index(CHAR_CODE, old_idx)
            old_idx = second_old + 1
            first_new = new.index(CHAR_CODE, new_idx)
            new_idx = first_new + 1
            second_new = new.index(CHAR_CODE, new_idx)
            new_idx = second_new + 1
            old_str = old[first_old:(second_old + 1)]
            new_str = new[first_new:(second_new + 1)]
            result = result.replace(new_str, old_str)

    return result

def correct_link_text(new_text):
    new = new_text
    start = 0
    end = 0
    while True:
        try:
            start = new.index("](", end)
            if start >= 0:
                end = new.index(")", start)
                if end >= 0:
                    link = new[start:(end+1)]
                    removed_white = link.replace(" ", "")
                    new = new.replace(link, removed_white)
                    diff_idx = len(link) - len(removed_white)
                    end = end - diff_idx
            else:
                break
        except:
            break
    return new

def correct_bold_text(new_text):
    new = new_text
    start = 0
    end = 0
    while True:
        try:
            start = new.index("**", end)
            if start >= 0:
                end = new.index("**", start + 1) +2
                if end >= 0:
                    link = new[start:(end)]
                    removed_white = link.replace(" ", "")
                    new = new.replace(link, removed_white)
                    diff_idx = len(link) - len(removed_white)
                    end = end - diff_idx
            else:
                break
        except:
            break
    return new


def translate(origin, dest_lang):
    print(origin)
    result = translate_client.translate(origin, target_language=dest_lang)
    translated = result['translatedText']
    translated = translated.replace('（', '(')
    translated = translated.replace('）', ')')
    translated = translated.replace('＃', '#')
    translated = translated.replace('【', '[')
    translated = translated.replace('】', ']')
    translated = translated.replace('&quot;', '"')
    translated = translated.replace('&gt;', '>')
    translated = translated.replace('「', '"')
    translated = translated.replace('」', '"')
    translated = translated.replace('&#39;', '`')
    translated = re.sub(r"^-", "- ", translated)
    translated = correct_code_text(origin, translated)
    translated = correct_link_text(translated)
    translated = correct_bold_text(translated)
    print(translated)
    return translated


def parse_metadata_and_translate(src_lines, des_lines, idx, dest_lang, url):
    text = src_lines[idx]
    if not text.startswith(SEPARATOR_META_DATA):
        return -1

    des_lines.append(text)
    idx = idx + 1
    start = idx
    for i in range(start, len(src_lines)):
        idx = i
        text = src_lines[idx]
        if text.startswith(SEPARATOR_META_DATA):
            idx = idx + 1
            des_lines.append(text)
            break
        elif text.startswith(STR_TITLE):
            title = text[len(STR_TITLE):-1].strip()
            translated = translate(title, dest_lang) + "\n"
            des_lines.append("%s %s"%(STR_TITLE, translated))
        elif text.startswith(STR_URL):
            url.append(text[len(STR_URL):-1].strip())
            des_lines.append(text.replace("msyksphinz.hatenablog.com", "fpgadevdiary.hatenadiary.com"))
        elif text.startswith(STR_EDITURL):
            url.append(text[len(STR_EDITURL):-1].strip())
            des_lines.append(text.replace("msyksphinz.hatenablog.com", "fpgadevdiary.hatenadiary.com"))
        elif text.startswith(STR_DESCRIPTION):
            description = text[len(STR_DESCRIPTION):-1].strip()
            translated = translate(description, dest_lang) + "\n"
            des_lines.append("%s %s"%(STR_DESCRIPTION, translated))
        else:
            des_lines.append(text)

    if (idx + 1) == len(src_lines):
        return -1

    return idx

def parse_body_and_translate(src_lines, des_lines, idx, dest_lang, url):
    start = idx
    is_in_code = False
    des_lines.append("Note: This sentense is translated by Google Translate.\n\n")
    des_lines.append("Original Japanese sentence is at " + url[0] + "\n\n")
    des_lines.append("---\n\n")

    for i in range(start, len(src_lines)):
        idx = i
        text = src_lines[i].strip()
        if text == "\n" or text == "":
            des_lines.append(src_lines[i])
        elif text.startswith(SEPARATOR_CODE):
            is_in_code = not is_in_code
            des_lines.append(src_lines[i])
        elif text.startswith(SEPARATOR_IMG):
            des_lines.append(src_lines[i])
        elif text.startswith(SEPARATOR_IMG_CODE):
            des_lines.append(src_lines[i])
        elif len(text) > 4 and text[0] == STR_STAR and text[1] == STR_STAR and text[-1] == STR_STAR and text[-2] == STR_STAR:
            des_lines.append(src_lines[i])
        elif is_english(text):
            des_lines.append(src_lines[i])
        else:
            if is_in_code:
                des_lines.append(src_lines[i])
            else:
                translated = translate(text, dest_lang) + "\n"
                des_lines.append(translated)

def parse_and_translate(src, des, lang):
    src_lines = []
    des_lines = []
    url = []

    with open(src, "r") as f:
        src_lines = f.readlines()

    idx = 0
    idx = parse_metadata_and_translate(src_lines, des_lines, idx, lang, url)
    idx = parse_body_and_translate(src_lines, des_lines, idx, lang, url)

    if src != des:
        with open(des, "w") as f:
            for line in des_lines:
                f.write(line)

if __name__ == "__main__":
    if len(sys.argv) != 4:
        print("please input src, des files and des lang")
        exit(1)

    src_file_name = sys.argv[1]
    des_file_name = sys.argv[2]
    des_lang = sys.argv[3]
    cur_dir = os.getcwd()

    for (root, dirs, files) in os.walk(cur_dir):
        src = root + "/" + src_file_name
        des = root + "/" + des_file_name
        if not os.path.exists(des) and os.path.exists(src):
            print(src)
            print(des)
            parse_and_translate(src, des, des_lang)
