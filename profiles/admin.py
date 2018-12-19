from django.contrib import admin
from .models import Post, Comment, Profile, Course, Resource

admin.site.register(Post)
admin.site.register(Comment)
admin.site.register(Profile)
admin.site.register(Course)
admin.site.register(Resource)
