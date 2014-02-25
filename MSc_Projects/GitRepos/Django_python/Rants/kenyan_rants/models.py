from django.db import models
from taggit.managers import TaggableManager
# Create your models here.

class Post(models.Model):
	title = models.CharField(max_length=100)
	body = models.TextField()
	date = models.DateTimeField()
	tags = TaggableManager()
	likes = models.IntegerField(default = 0)

	def __unicode__(self):
		return self.title




