#! /usr/bin/env python

from django import forms
from models import Post

class FormBlog(forms.ModelForm):
	class Meta:
		model = Post


