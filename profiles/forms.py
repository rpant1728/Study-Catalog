from django import forms
from django.forms import ModelForm
from django.contrib.auth.models import User
from .models import Profile, Resource, Course

class ProfileForm(ModelForm):
    class Meta:
        model = Profile
        exclude = ['user', 'admin', 'admin_request', 'admin_of_courses']
        widgets = {
            'first_name': forms.TextInput(attrs={'placeholder': 'First Name', 'class' : 'form-field text'}),
            'last_name': forms.TextInput(attrs={'placeholder': 'Last Name', 'class' : 'form-field text'}),
            'age': forms.TextInput(attrs={'placeholder': 'Age', 'class' : 'form-field text'}),
            'city': forms.TextInput(attrs={'placeholder': 'City', 'class' : 'form-field text'}),
            'degree': forms.Select(attrs={'class' : 'form-field choice'}),
            'department': forms.Select(attrs={'class' : 'form-field choice'}),
            'description': forms.Textarea(attrs={'placeholder': 'Enter description here', 'class' : 'form-field textarea'}),
        }

class ResourceForm(ModelForm):
    def __init__(self, courses, *args,**kwargs):
        super(ResourceForm, self).__init__(*args, **kwargs) 
        self.fields['course'].queryset = courses 
        self.fields['course'].empty_label = "Select course"
    
    class Meta:
        model = Resource
        exclude = ['user', 'uploaded_on']
        widgets = {
            'title': forms.TextInput(attrs={'placeholder': 'Enter suitable title', 'class' : 'form-field text'}),
            'description': forms.Textarea(attrs={'placeholder': 'Enter description', 'class' : 'form-field textarea'}),
            'course': forms.Select(attrs={'class' : 'form-field choice'}),
            'file': forms.FileInput(attrs={'class' : 'form-field file'}),
        }

class CourseForm(ModelForm):
    def __init__(self, *args,**kwargs):
        super(CourseForm, self).__init__(*args, **kwargs) 
        self.fields['department'].empty_label = "Select department"
        self.fields['degree'].empty_label = "Select degree"

    class Meta:
        model = Course
        exclude = ['approved']
        widgets = {
            'name': forms.TextInput(attrs={'placeholder': 'Enter course title', 'class' : 'form-field text'}),
            'code': forms.TextInput(attrs={'placeholder': 'Enter course code (optional)', 'class' : 'form-field text'}),
            'department': forms.Select(attrs={'class' : 'form-field choice'}),
            'degree': forms.Select(attrs={'class' : 'form-field choice'}),
        }