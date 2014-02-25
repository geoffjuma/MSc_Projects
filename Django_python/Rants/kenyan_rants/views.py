from django.shortcuts import render_to_response
from kenyan_rants.models import Post.model
from django.http import HttpResponseRedirect
from forms import FormBlog 
from django.http import HttpResponse
from django.core.context_processors import csrf 

def blogs(request):
	return render_to_response('blog.html',
				{'blogs':Post.objects.all()})

def article(request,article_id = 1):
	return render_to_response('article.html')

def blogpost():
	if request.POST:
		form  = FormBlog(request.POST)
		if form.is_valid():
			form.save()
			return HttpResponseRedirect('/articles/all')
		else:
			form = FormBlog()
		args = {}
		args.update(csrf(request))
		args['form'] = form
		
		return render_to_response('mypost.html',args) 
	
# Create your views here.
