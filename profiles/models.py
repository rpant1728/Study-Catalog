from django.db import models
import uuid
from django.contrib.auth.models import User
# Create your models here.

class Post(models.Model):
    title = models.CharField(max_length=255)
    content = models.TextField()
    created_on = models.DateTimeField(auto_now_add=True)
    author = models.ForeignKey(User, related_name='author',on_delete=models.CASCADE)
    vote = models.ManyToManyField(User, related_name='upvotes')
    vote_count = models.IntegerField(default = 0)
    def __str__(self):
        return "%s (%s)" % (self.title, self.author.username)

class Comment(models.Model):
    content = models.TextField()
    created_on = models.DateTimeField(auto_now_add=True)
    author = models.ForeignKey(User,on_delete=models.CASCADE)
    post = models.ForeignKey(Post, related_name='comments', on_delete=models.CASCADE)
