#!/usr/bin/env ruby
# -*- encoding: utf-8 -*-

require 'atomutil'

POST_URI = "https://blog.hatena.ne.jp/msyksphinz/msyksphinz.hatenablog.com/atom/entry"

auth = Atompub::Auth::Wsse.new(
  username: 'msyksphinz',
  password: 'l5l3j1mx7h'
)
client = Atompub::Client.new(auth: auth)

feed = client.get_feed(POST_URI);

out_fp = File.open("contents.md", "w")

while feed != nil do
  entry = feed.entries
  entry.each{|content|
    out_fp.printf("===\n")
    out_fp.printf("- [%s (%s)](%s)\n", content.title, content.updated.strftime("%y/%m/%d"), content.links[1].href)
    out_fp.printf("%s\n", content.content)
    out_fp.printf("===\n")
  }

  if feed.next_link == nil then
    break
  else
    feed = client.get_feed(feed.next_link);
  end
end

out_fp.close
