from django.conf.urls import patterns, include, url

# Uncomment the next two lines to enable the admin:
from django.contrib import admin
admin.autodiscover()

urlpatterns = patterns('',
    # Examples:
    # url(r'^$', 'Rants.views.home', name='home'),
     url(r'^kenyan_rants/', include('kenyan_rants.urls')),

     url(r'^admin/', include(admin.site.urls)),
)
