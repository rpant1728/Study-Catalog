from django import forms

class PostForm(forms.Form):
    title = forms.CharField(max_length=1000, help_text="Enter title!", widget=forms.Textarea)
    data = forms.CharField(max_length=1000, help_text="Write your post!", widget=forms.Textarea)

class CommentForm(forms.Form):
    data = forms.CharField(max_length=1000)
    post_id = forms.IntegerField()