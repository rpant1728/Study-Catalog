from django.conf.urls import url, include
from django.contrib import admin
from django.contrib.auth import views as auth_views
from . import views

urlpatterns = [
    url('', include('django.contrib.auth.urls')) ,
    url(r'^auth/', include('social_django.urls', namespace='social')),  
    url(r'^$', views.home, name='home'),
    url(r'^post/', views.post_new, name='post_new'),
    url(r'^comment/', views.comment_new, name='comment_new'),
    url(r'^votes/', views.post_votes, name='post_votes'),
    url(r'^posts/(?P<pk>\d+)$', views.user_posts, name='user_posts'),
    url(r'^edit-profile', views.edit_profile, name='edit-profile'),
    url(r'^profile/(?P<pk>\d+)$', views.profile_detail, name='profile-detail'),
    url(r'^p(?P<pk>\d+)$', views.post_detail, name='post-detail'),
    url(r'^upload-resource/', views.upload_resource, name='upload-resource'),
]
