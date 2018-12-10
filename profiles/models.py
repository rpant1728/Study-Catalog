from django.db import models
import uuid
from django.urls import reverse
from django.contrib.auth.models import User

class Post(models.Model):
    title = models.CharField(max_length=255)
    content = models.TextField()
    created_on = models.DateTimeField(auto_now_add=True)
    author = models.ForeignKey(User, related_name='author',on_delete=models.CASCADE)
    vote = models.ManyToManyField(User, related_name='upvotes')
    vote_count = models.IntegerField(default = 0)
    
    def get_absolute_url(self):
        return reverse('post-detail', args=[str(self.id)])

    def __str__(self):
        return "%s (%s)" % (self.title, self.author.username)

class Comment(models.Model):
    content = models.TextField()
    created_on = models.DateTimeField(auto_now_add=True)
    author = models.ForeignKey(User,on_delete=models.CASCADE)
    post = models.ForeignKey(Post, related_name='comments', on_delete=models.CASCADE)

class Profile(models.Model):
    DEPARTMENTS = (
        ('CSE', 'Computer Science and Engineering'),
        ('CE', 'Chemical Engineering'),
        ('BT', 'Bio-Technology'),
        ('CST', 'Chemical Science & Technology'),
        ('CE', 'Civil Engineering'),
        ('ECE', 'Electronics & Communication Engineering'),
        ('EEE', 'Electronics & Electrical Engineering'),
        ('EP', 'Engineering Physics'),
        ('ME', 'Mechanical Engineering'),
        ('MNC', 'Mathematics & Computing'),
    )
    DEGREES = (
        ('PhD', 'Doctorate of Philosophy'), 
        ('B.Tech', 'Bachelor of Technology'), 
        ('M.Tech', 'Master of Technology'), 
        ('B.Des', 'Bachelor of Design'),
        ('M.Des', 'Master of Design')
    )
    department = models.CharField(max_length=3, choices=DEPARTMENTS, blank=True, help_text='Department')
    age = models.IntegerField()
    first_name = models.CharField(max_length=255)
    last_name = models.CharField(max_length=255)
    degree = models.CharField(max_length=6, choices=DEGREES, blank=True, help_text='Degree')
    description = models.CharField(max_length=255, blank=True)
    profile_picture = models.ImageField(upload_to='images/profile_pictures',
                                        default="images/profile_pictures/default.png")
    city = models.CharField(max_length=255)
    user = models.ForeignKey(User, related_name='profile', on_delete=models.CASCADE, null=True)

    def get_absolute_url(self):
        return reverse('profile-detail', args=[str(self.id)])

    def __str__(self):
        return "%s %s" % (self.first_name, self.last_name)


