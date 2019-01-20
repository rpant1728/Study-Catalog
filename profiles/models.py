from django.db import models
import uuid
from django.urls import reverse
from django.contrib.auth.models import User

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
        ('OT', 'Other')
    )

DEGREES = (
        ('PhD', 'Doctorate of Philosophy'), 
        ('B.Tech', 'Bachelor of Technology'), 
        ('M.Tech', 'Master of Technology'), 
        ('B.Des', 'Bachelor of Design'),
        ('M.Des', 'Master of Design')
    )

class Post(models.Model):
    title = models.CharField(max_length=255)
    content = models.TextField()
    created_on = models.DateTimeField(auto_now_add=True)
    author = models.ForeignKey(User, related_name='author',on_delete=models.CASCADE)
    upvote = models.ManyToManyField(User, related_name='upvotes')
    downvote = models.ManyToManyField(User, related_name='downvotes')
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

class Course(models.Model):
    department = models.CharField(max_length=3, choices=DEPARTMENTS, blank=True, help_text='Department')
    degree = models.CharField(max_length=6, choices=DEGREES, blank=True, help_text='Degree')
    name = models.CharField(max_length=200)
    code = models.CharField(max_length=20, blank=True)
    approved = models.BooleanField(default=False)
    admin = models.ForeignKey(User, related_name='admin', on_delete=models.CASCADE, null=True)
    def __str__(self):
        return "%s" % self.name

class Profile(models.Model):
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
    admin_of_courses = models.ManyToManyField(Course, related_name='courses')
    admin = models.BooleanField(default=False)
    admin_request = models.BooleanField(default=False)

    def get_absolute_url(self):
        return reverse('profile-detail', args=[str(self.id)])

    def __str__(self):
        return "%s %s" % (self.first_name, self.last_name)

class Folder(models.Model):
    title = models.CharField(max_length=200, default='New Folder')
    # folders = models.ManyToManyField("self", symmetrical=False)
    parent_folder = models.ForeignKey("self", related_name="subfolders", on_delete=models.CASCADE, null=True, blank=True)
    course = models.ForeignKey(Course, related_name="folders", on_delete=models.CASCADE)
    created_on = models.DateTimeField(auto_now_add=True)
    root = models.BooleanField(default=False)

    def __str__(self):
        return "%s" % self.title

class Resource(models.Model):
    title = models.CharField(max_length=200)
    tag = models.CharField(max_length=200)
    course = models.ForeignKey(Course, related_name='resources', on_delete=models.CASCADE)
    file = models.FileField(upload_to='resources/')
    user = models.ForeignKey(User, related_name='user', on_delete=models.CASCADE)
    uploaded_on = models.DateTimeField(auto_now_add=True)
    approved = models.BooleanField(default=False)
    folder = models.ForeignKey(Folder, related_name="resource", on_delete=models.CASCADE)

    def __str__(self):
        return "%s" % self.title

