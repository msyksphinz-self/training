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

out_fp = File.open("output.txt", "w")

while feed != nil do
  entry = feed.entries
  entry.each{|content|
    out_fp.printf("<ul><li><a href=\"%s\">%s</a></li></ul>\n", content.links[1].href, content.title)
  }

  if feed.next_link == nil then
    break
  else
    feed = client.get_feed(feed.next_link);
  end
end

out_fp.close
