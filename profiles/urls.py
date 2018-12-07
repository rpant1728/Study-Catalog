from django.conf.urls import url, include
from django.contrib import admin
from django.contrib.auth import views as auth_views
from . import views

urlpatterns = [
    url('', include('django.contrib.auth.urls')) ,
    url(r'^auth/', include('social_django.urls', namespace='social')),  # <- Here
    url(r'^$', views.home, name='home'),
    url(r'^post/', views.post_new, name='post_new'),
    url(r'^comment/', views.comment_new, name='comment_new'),
    url(r'^profile/', views.profile_view, name='profile'),
    url(r'^upvote/', views.upvote, name='post_upvote'),
     url(r'^downvote/', views.downvote, name='post_downvote')
]