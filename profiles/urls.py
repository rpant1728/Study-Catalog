from django.conf.urls import url, include
from django.contrib import admin
from django.contrib.auth import views as auth_views
from . import views

urlpatterns = [
    url('', include('django.contrib.auth.urls')) ,
    url(r'^auth/', include('social_django.urls', namespace='social')),  # <- Here
    url(r'^$', views.home, name='home'),
]