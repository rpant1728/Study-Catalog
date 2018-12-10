from django.shortcuts import render, render_to_response, redirect
from django.contrib.auth.models import User
from django.utils import timezone
from profiles.models import Post, Comment, Profile
from profiles.forms import PostForm, CommentForm, ProfileForm
from django.views import generic
from django.urls import reverse
from django.http import HttpResponseRedirect


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
			post.vote_count = 0
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

def user_posts(request, pk):
	user = User()
	user = User.objects.get(id=pk)
	posts = Post.objects.filter(author=user)
	template_data = {'posts' : posts, 'profile': True}
	return render(request, 'base.html', template_data)

def upvote(request):
	if request.method == "POST":
		post_id = request.POST['post_id']
		page = request.POST['page']
		post = Post.objects.get(id=post_id)
		post.vote.add(request.user)
		post.vote_count = post.vote_count + 1
		post.save()
		if page == "user_posts":
			return redirect(page, request.user.id)
		else:
			return redirect(page, post.id)
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
		if page == "user_posts":
			return redirect(page, request.user.id)
		else:
			return redirect(page, post.id)
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts}
	return render(request, 'base.html', template_data)

def profile_detail(request, pk):
	profile = Profile.objects.filter(user_id=pk)
	if profile.count() == 0:
		return HttpResponseRedirect(reverse("edit-profile"))
	return render(request, 'profiles/profile_detail.html', context={'profile': profile[0]})    

def edit_profile(request):
	user = request.user
	profile = Profile(user_id=user.id)
	if request.method == "POST":
		form = ProfileForm(request.POST, request.FILES, instance=profile)
		if form.is_valid():
			form.save()
			return HttpResponseRedirect(reverse("profile-detail", args=(user.id,)))
	else:
		form = ProfileForm()
	return render(request, 'profile_form.html', context={'form': form})

def post_detail(request, pk):
	post = Post.objects.filter(pk=pk)
	if post.count() == 0:
		return HttpResponseRedirect(reverse("post_new"))
	return render(request, 'post/post_detail.html', context={'post': post[0]}) 