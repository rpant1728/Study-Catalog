from django.shortcuts import render, render_to_response, redirect
from django.contrib.auth.models import User
from django.utils import timezone
from profiles.models import Post, Comment, Profile, Course, Resource
from profiles.forms import ProfileForm, ResourceForm, CourseForm
from django.views import generic
from django.urls import reverse
from django.http import HttpResponse, HttpResponseRedirect
from django.db.models import Q
from django.db import connection

def home(request):
	profile = Profile.objects.filter(user_id=request.user.id)
	if profile.count() == 0:
		return HttpResponseRedirect(reverse("edit-profile"))
	all_posts = Post.objects.all()
	courses = Course.objects.filter(Q(department=profile[0].department) | Q(department__contains="OT"))
	resource_form = ResourceForm(courses)
	course_form = CourseForm()
	template_data = {'posts' : all_posts, 'res_form': resource_form, 'course_form': course_form}
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
			profile.save()
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
	profile = Profile.objects.filter(user=request.user)
	courses = Course.objects.filter(department=profile[0].department)
	user = request.user
	resource = Resource(user=user, uploaded_on=timezone.now())
	if request.method == "POST":
		form = ResourceForm(courses, request.POST, request.FILES, instance=resource)
		if form.is_valid():
			form.save()
	return HttpResponseRedirect(reverse("home"))

def create_course(request):
	course = Course(approved=False)
	if request.method == "POST":
		form = CourseForm(request.POST, instance=course)
		if form.is_valid():
			new_course = form.save()
			profile = Profile.objects.filter(user=request.user).first()	
			profile.admin_of_courses.add(new_course)
			profile.save()
	return HttpResponseRedirect(reverse("home"))

def catalog(request):
	profile = Profile.objects.filter(user=request.user).first()
	resources = Resource.objects.filter(course__department__contains=profile.department)
	template_data = dict()
	template_data['resources'] = resources
	all_course_requests = Course.objects.filter(approved=False)
	all_resource_requests = Resource.objects.filter(approved=False)
	all_admin_requests = Profile.objects.filter(admin_request=True)
	if profile.admin:
		template_data['is_admin'] = True
		template_data['course_requests'] = all_course_requests
		template_data['resource_requests'] = all_resource_requests
		template_data['admin_requests'] = all_admin_requests
		return render(request, 'catalog.html', template_data)
	if profile.admin_of_courses.all().count() > 0:
		template_data['is_admin'] = True
		resource_requests = []
		for resource in all_resource_requests:
			for course in profile.admin_of_courses.all():
				if resource.course == course:
					resource_requests.append(resource)	
		template_data['resource_requests'] = resource_requests
		return render(request, 'catalog.html', template_data)
	return render(request, 'catalog.html', template_data)

def approve_course(request):
	template_data = dict()
	if request.method == "POST":
		course_id = request.POST['course_id']
		course = Course.objects.get(id=course_id)
		course.approved = True
		course.save()
	return HttpResponseRedirect(reverse("catalog"))

def reject_course(request):
	template_data = dict()
	if request.method == "POST":
		course_id = request.POST['course_id']
		course = Course.objects.get(id=course_id)
		profiles = Profile.objects.all()
		for profile in profiles:
			if course in profile.admin_of_courses.all():
				profile.admin_of_courses.remove(course)
		course.delete()
	return HttpResponseRedirect(reverse("catalog"))

def approve_resource(request):
	template_data = dict()
	if request.method == "POST":
		resource_id = request.POST['resource_id']
		resource = Resource.objects.get(id=resource_id)
		resource.approved = True
		resource.save()
	return HttpResponseRedirect(reverse("catalog"))

def reject_resource(request):
	template_data = dict()
	if request.method == "POST":
		resource_id = request.POST['resource_id']
		resource = Resource.objects.get(id=resource_id)
		resource.delete()
	return HttpResponseRedirect(reverse("catalog"))

def admin_request(request):
	template_data = dict()
	if request.method == "POST":
		user_id = request.POST['user_id']
		user = Profile.objects.get(user=user_id)
		user.admin_request = True
		user.save()
	return HttpResponseRedirect(reverse("home"))

def approve_admin(request):
	template_data = dict()
	if request.method == "POST":
		user_id = request.POST['user_id']
		user = Profile.objects.get(user=user_id)
		user.admin_request = False
		user.admin = True
		user.save()
	return HttpResponseRedirect(reverse("catalog"))

def reject_admin(request):
	template_data = dict()
	if request.method == "POST":
		user_id = request.POST['user_id']
		user = Profile.objects.get(user=user_id)
		user.admin_request = False
		user.save()
	return HttpResponseRedirect(reverse("catalog"))