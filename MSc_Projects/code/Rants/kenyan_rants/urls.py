from django.conf.urls import patterns, include, url
from django.views.generic import ListView,DetailView
from kenyan_rants.models import Post

urlpatterns = patterns('',
   
	url(r'^$', 
			ListView.as_view(
			queryset = Post.objects.all().order_by("date")[:10],
			template_name="index.html")),

	url(r'^(?P<pk>\d+)$', DetailView.as_view(
                           model=Post,
                           template_name="post.html")),

	url(r'^archives/$', ListView.as_view(
                           queryset=Post.objects.all().order_by("date"),
                           template_name="archives.html")),

	url(r'^latest/$', ListView.as_view(
                           queryset=Post.objects.all().order_by("date")[:25],
                           template_name="latest.html")),
	url(r'^postblog/$','kenyan_rants.views.postblog'),
)
             
