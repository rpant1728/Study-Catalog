from django.shortcuts import render, render_to_response, redirect
from django.contrib.auth.models import User
from django.utils import timezone
from profiles.models import Post
from profiles.forms import PostForm
from profiles.models import Comment
from profiles.forms import CommentForm

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
			post.upvote_count = 0
			post.save()
			template_data['post_form'] = form
			return render(request, 'base.html', template_data)
		else:
			form = PostForm()	
			template_data['post_form'] = form
	else:
		form = PostForm()
		template_data['post_form'] = form
	return render(request, 'base.html', template_data)

def comment_new(request):
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts}
	post_form = PostForm()
	template_data['post_form'] = post_form
	if request.method == "POST":
		comment_form = CommentForm(request.POST)
		if comment_form.is_valid():
			print(comment_form.cleaned_data)
			comment = Comment()
			comment.post = Post.objects.get(id=comment_form.cleaned_data.get('post_id'))
			comment.author = request.user
			comment.created_on = timezone.now()
			comment.content = comment_form.cleaned_data['data']
			comment.save()
			template_data['comment_form'] = comment_form
			return render(request, 'base.html', template_data)
		else:
			comment_form = CommentForm()	
			template_data['comment_form'] = comment_form
	else:
		comment_form = CommentForm()
		template_data['comment_form'] = comment_form
	return render(request, 'base.html', template_data)

def profile_view(request):
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts, 'profile': True}
	return render(request, 'base.html', template_data)

def upvote(request):
	if request.method == "POST":
		post_id = request.POST['post_id']
		page = request.POST['page']
		post = Post.objects.get(id=post_id)
		post.vote.add(request.user)
		post.vote_count = post.vote_count + 1
		post.save()
		return redirect(page)
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts}
	return render(request, 'base.html', template_data)

def downvote(request):
	if request.method == "POST":
		post_id = request.POST['post_id']
		page = request.POST['page']
		post = Post.objects.get(id=post_id)
		post.vote.add(request.user)
		post.vote_count = post.vote_count - 1
		post.save()
		return redirect(page)
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts}
	return render(request, 'base.html', template_data)