from django import forms
class PostForm(forms.Form):
    title=forms.CharField(max_length=10000, help_text="Enter title!", widget=forms.Textarea)
    data=forms.CharField(max_length=10000, help_text="Write your post!", widget=forms.Textarea)