from django.shortcuts import render, render_to_response
from django.utils import timezone
from profiles.models import Post
from profiles.forms import PostForm

def home(request):
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts}
	return render(request, 'base.html', template_data)

def post_new(request):
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts}
	if request.method == "POST":
		form = PostForm(request.POST)
		if form.is_valid():
			post = Post()
			post.title = form.cleaned_data['title']
			post.author = request.user
			post.created_on = timezone.now()
			post.content = form.cleaned_data['data']
			post.save()
			template_data['form'] = form
			return render(request, 'base.html', template_data)
		else:
			form = PostForm()	
			template_data['form'] = form
	else:
		form = PostForm()
		template_data['form'] = form
	return render(request, 'base.html', template_data)