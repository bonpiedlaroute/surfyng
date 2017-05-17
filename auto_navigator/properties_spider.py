import scrapy


class PropertiesSpider(scrapy.Spider):
   name = "properties_spider"

   def start_requests(self):
      urls = [
      'http://www.seloger.com/list.htm?tri=initial&idtypebien=2&idtt=2&div=2238'
      ]
      for url in urls:
         yield scrapy.Request(url=url, callback=self.parse)

   def parse(self, response):
      links = response.xpath('//section[@class="liste_resultat"]').xpath('.//a[contains(@href, "div=2238")]/@href').extract()
      announces = []
      excluded = []
      for url in links:
         if "annonces" in url:
            announces.append(url)
         else:
            excluded.append(url)

      print "announces found:\n"
      for url in announces:
         print "%s:" % url
      print "\n announces excluded:\n"
      for url in excluded:
         print "%s:" %url
