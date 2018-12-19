from django.shortcuts import render, render_to_response, redirect
from django.contrib.auth.models import User
from django.utils import timezone
from profiles.models import Post, Comment, Profile, Course, Resource
from profiles.forms import ProfileForm, ResourceForm
from django.views import generic
from django.urls import reverse
from django.http import HttpResponse, HttpResponseRedirect

def home(request):
	profile = Profile.objects.filter(user_id=request.user.id)
	if profile.count() == 0:
		return HttpResponseRedirect(reverse("edit-profile"))
	all_posts = Post.objects.all()
	courses = Course.objects.filter(department=profile[0].department)
	user = request.user
	form = ResourceForm(courses)
	template_data = {'posts' : all_posts, 'form': form}
	return render(request, 'home.html', template_data)

def post_new(request):
	if request.method == "POST":
		post = Post()
		post.title = request.POST['title']
		post.author = request.user
		post.created_on = timezone.now()
		post.content = request.POST['data']
		post.upvote_count = 0
		post.save()
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts}
	return render(request, 'post.html', template_data)

def comment_new(request):
	if request.method == "POST":
		comment = Comment()
		post_id = request.POST['post-id']
		comment.post = Post.objects.get(id=post_id)
		comment.author = request.user
		comment.created_on = timezone.now()
		comment.content = request.POST['data']
		comment.save()
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts}
	return render(request, 'home.html', template_data)

def user_posts(request, pk):
	user = User()
	user = User.objects.get(id=pk)
	posts = Post.objects.filter(author=user)
	template_data = {'posts' : posts, 'profile': True}
	return render(request, 'home.html', template_data)

def post_votes(request):
	if request.method == "POST":
		post_id = request.POST['post_id']
		upvote = request.POST['upvote']
		downvote = request.POST['downvote']
		down_remove = request.POST['down_remove']
		up_remove = request.POST['up_remove'] 
		post = Post.objects.get(id=post_id)
		if(upvote == "1"):
			post.upvote.add(request.user)
			post.vote_count = post.vote_count + 1
		if(up_remove == "1"):
			post.upvote.remove(request.user)
			post.vote_count = post.vote_count - 1
		if(down_remove == "1"):
			post.downvote.remove(request.user)
			post.vote_count = post.vote_count + 1			
		if(downvote == "1"):
			post.downvote.add(request.user)
			post.vote_count = post.vote_count - 1
		post.save()
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts}
	return render(request, 'home.html', template_data)

def profile_detail(request, pk):
	profile = Profile.objects.filter(user_id=pk)
	if profile.count() == 0:
		return HttpResponseRedirect(reverse("edit-profile"))
	user = User()
	user = User.objects.get(id=pk)
	posts = Post.objects.filter(author=user)
	template_data = {'posts' : posts, 'profile': profile[0]}
	return render(request, 'profiles/profile_detail.html', template_data)  

def edit_profile(request):
	user = request.user
	if not request.user.is_authenticated:
		return render(request, 'profile_form.html')
	profile = Profile(user=user)
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

def upload_resource(request):
	profile = Profile.objects.filter(user_id=request.user.id)
	courses = Course.objects.filter(department=profile[0].department)
	user = request.user
	resource = Resource(user=user, uploaded_on=timezone.now())
	if request.method == "POST":
		form = ResourceForm(courses, request.POST, request.FILES, instance=resource)
		print(user.id)
		if form.is_valid():
			form.save()
			print(user.id)
	return HttpResponseRedirect(reverse("home"))
