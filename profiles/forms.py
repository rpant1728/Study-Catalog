from django import forms
from django.forms import ModelForm
from django.contrib.auth.models import User
from .models import Profile

class PostForm(forms.Form):
    title = forms.CharField(max_length=1000, help_text="Enter title!", widget=forms.Textarea)
    data = forms.CharField(max_length=1000, help_text="Write your post!", widget=forms.Textarea)

class CommentForm(forms.Form):
    data = forms.CharField(max_length=1000)
    post_id = forms.IntegerField()

class ProfileForm(ModelForm):
    class Meta:
        model = Profile
        exclude = ['user']
