from django.conf.urls import url, include
from django.contrib import admin
from django.contrib.auth import views as auth_views
from . import views

urlpatterns = [
    url('', include('django.contrib.auth.urls')) ,
    url(r'^auth/', include('social_django.urls', namespace='social')),  
    url(r'^$', views.home, name='home'),
    url(r'^post/', views.post_new, name='post_new'),
    url(r'^post-delete/', views.post_delete, name='post_delete'),
    url(r'^comment/', views.comment_new, name='comment_new'),
    url(r'^comment-delete/', views.comment_delete, name='comment_delete'),
    url(r'^votes/', views.post_votes, name='post_votes'),
    url(r'^posts/(?P<pk>\d+)$', views.user_posts, name='user_posts'),
    url(r'^edit-post/', views.edit_posts, name='edit_post'),
    url(r'^edit-profile', views.edit_profile, name='edit-profile'),
    url(r'^profile/(?P<pk>\d+)$', views.profile_detail, name='profile-detail'),
    url(r'^p(?P<pk>\d+)$', views.post_detail, name='post-detail'),
    url(r'^upload-resource/', views.upload_resource, name='upload-resource'),
    url(r'^create-course/', views.create_course, name='create-course'),
    url(r'^catalog/$', views.catalog, name='catalog'),
    url(r'^catalog/course/(?P<pk>\d+)$', views.course_detail, name='course-detail'),
    url(r'^catalog/course/(?P<pk>\d+)/folder/(?P<pk1>\d+)/$', views.folder_detail, name='folder-detail'),
    url(r'^approve-course/', views.approve_course, name='approve-course'),
    url(r'^reject-course/', views.reject_course, name='reject-course'),
    url(r'^approve-resource/', views.approve_resource, name='approve-resource'),
    url(r'^reject-resource/', views.reject_resource, name='reject-resource'),
    url(r'^approve-admin/', views.approve_admin, name='approve-admin'),
    url(r'^reject-admin/', views.reject_admin, name='reject-admin'),
    url(r'^admin-request/', views.admin_request, name='admin-request'),
    url(r'^create-folder/', views.create_folder, name='create-folder'),
    url(r'^folders/$', views.folders, name='folders'),
    url(r'^search/$', views.SearchListView, name='search_list_view'),
    url(r'^delete-resource/(?P<pk>\d+)$', views.delete_resource, name='delete-resource'),
    url(r'^delete-folder/(?P<pk>\d+)$', views.delete_folder, name='delete-folder'),
]