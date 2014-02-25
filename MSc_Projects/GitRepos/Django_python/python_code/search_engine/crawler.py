#!/usr/bin/env python

import re 
import sys 
import urlparse 
import urllib
'''get the page '''
'''page = 'http://www.yahoo.com'''
'''we are defining fecther class'''

class Fetcher(object):
	def __init__(self,url,link):
		self.url = url
		self.link = link


	def fetch(self):
		self.url = url 
		try:
			return urllib.urlopen(url).read()
		except:
			return "error somwehere! ninconmpoop"

	def get_next_page(self,web_page):
		self.web_page = web_page
		start_link = web_page.find('<a href =' )
		if start_link == -1:
			return None, 0
		print ('No links found here')

		start_quote = web_page.find('"',start_link)
		end_quote = web_page.find('"',start_quote + 1)
		url = page[start_quote + 1:end_quote]
		return url, end_quote

	def get_all_links(self,page):
		self.page = page
		links = []
		while True:
			url,endpos = get_next_page(page)
			if url :
				links.append(url)
				page = page[endpos:]
			else:
			 	break

		return links

'''Lets define a crawler class with its methods'''

class Crawler(object):
	def __init__(self, url,index,keyword):
		self.link = []
		self.url = url
		self.index = index
		self.keyword = keyword

	def union(self,a,b):
		for e in b:
			if e not in a:
				a.append(e)

	def clicks(index, keyword,url):
		urls = lookup(index,keyword)
		if urls:
			for entry in urls:
					if entry[0] == urls:
						entry[1] = entry[1]+1

	def add_to_index(index,keyword,url):
		for entry in index:
			if entry[0] == keyword:
				for element in entry[1]:
					if element[0] == url:
						return
					else:
						entry[1].append([url,0])
				return 
		index.append([keyword,[[index,0]]])

	def lookup(index,keyword):
		result = []
		for entry in index:
			if entry[0] == keyword:
				result.append(entry[1])
		return result
	
	def add_page_to_index(index,url,content):
		words = content.split()
		for word in words:
			add_to_index(index,word,url)

	
	def crawl_web(self, url):
		self.seed = url
		tocrawl = [url]
		crawled = []
		index = []
		while tocrawl:
			page = tocrawl.pop()
			if page not in crawled:
				content = fetch(page)
				add_page_to_index(index,page,content)
				union(tocrawl,get_all_links(content))
				crawled.append(page)
		return index 

def getLinks(url):
	page = Fetcher()
	page.fetch()
        for i, url in enumerate(page):
		print "%d. %s" % (i, url)


def main():
	getLinks("http://www.yahoo.com")
	print "Crawling %s " % url
 	crawler = Crawler()
	crawler.crawl_web(url)
	print "\n".join(crawler.urls)
	print "Found:    %d" % crawler.links
     	print "Followed: %d" % crawler.crawled

	if __name__ == "__main__":
	     main()

'''crawl_web('http://www.yahoo.com')'''

