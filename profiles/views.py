from django.shortcuts import render, render_to_response, redirect
from django.contrib.auth.models import User
from django.utils import timezone
from profiles.models import Post, Comment, Profile, Resource, Course, Folder
from profiles.forms import ProfileForm, ResourceForm, CourseForm
from django.views import generic
from django.urls import reverse
from django.http import HttpResponse, HttpResponseRedirect
from django.db.models import Q
from django.db import connection
from django.contrib.postgres.search import SearchQuery, SearchRank, SearchVector

def home(request):
	profile = Profile.objects.filter(user_id=request.user.id)
	user = request.user
	if not request.user.is_authenticated:
		all_posts = Post.objects.all()
		courses = Course.objects.all()
		course_form = CourseForm()
		template_data = {'posts' : all_posts, 'courses': courses, 'course_form': course_form}
		return render(request, 'home.html', template_data)
	if profile.count() == 0:
		return HttpResponseRedirect(reverse("edit-profile"))
	all_posts = Post.objects.all()
	courses = Course.objects.all()
	course_form = CourseForm()
	template_data = {'posts' : all_posts, 'courses': courses, 'course_form': course_form}
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

def post_delete(request):
	if request.method == "POST":
		post_id = request.POST['post_id']
		post = Post.objects.get(id=post_id)
		comments = Comment.objects.filter(post=post)
		for comment in comments.all():
			comment.delete()
		post.delete()
	all_posts = Post.objects.all()
	template_data = {'posts' : all_posts}
	return render(request, 'home.html', template_data)

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
	form = ResourceForm(request.POST, request.FILES)
	if form.is_valid():
		for file1 in request.FILES.getlist('files'):
			resource = Resource(file=file1, approved=False, user=request.user)
			resource.title = file1.name
			resource.course = Course.objects.get(id=request.POST['course'])
			resource.folder = Folder.objects.get(id=request.POST['folder'])
			resource.save()
	return HttpResponseRedirect(reverse("folder-detail", args=(request.POST['course'], request.POST['folder'],)))

def create_course(request):
	course = Course(approved=False, admin=request.user)
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
	courses = Course.objects.filter(approved=True)
	template_data = dict()
	template_data['courses'] = courses
	all_course_requests = Course.objects.filter(approved=False)
	all_resource_requests = Resource.objects.filter(approved=False)
	all_admin_requests = Profile.objects.filter(admin_request=True)
	if profile.admin:
		template_data['is_admin'] = True
		template_data['all_admin'] = True
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

def create_folder(request):
	folder = Folder()
	folder.title = request.POST['title']
	if folder.title == '':
		folder.title = "New Folder"
	folder.course = Course.objects.get(id=request.POST['course_id'])
	if not 'folder_id' in request.POST:
		folder.root = True
	folder.save()
	if 'folder_id' in request.POST:
		curr_folder = Folder.objects.get(id=request.POST['folder_id'])
		curr_folder.folders.add(folder)
	return HttpResponse("success")

def folders(request):
	course = Course.objects.filter(name=request.POST['course'])[0]
	folders = course.folders.all()
	return HttpResponse(folders)

def course_detail(request, pk):
	form = ResourceForm()
	course = Course.objects.get(id=pk)
	folders = course.folders.filter(root=True)
	template_data = {'folders' : folders, 'course': course, 'root': True, 'form': form}
	return render(request, 'filesystem.html', template_data)  

def folder_detail(request, pk, pk1):
	form = ResourceForm()
	profile = Profile.objects.filter(user=request.user).first()
	course = Course.objects.get(id=pk)
	folder = Folder.objects.get(id=pk1)
	folders = folder.folders.all()
	resources = Resource.objects.filter(folder__id=folder.id, approved=True)
	template_data = {'folders' : folders, 'resources' : resources, 'course': course, 'root': False, 'folder': folder, 'form': form}
	if profile.admin:
		template_data['all_admin'] = True
	else:
		template_data['all_admin'] = False
	return render(request, 'filesystem.html', template_data) 


def SearchListView(request):
	qs = Post.objects.all()
	qsr = Resource.objects.all()
	keywords = request.GET.get('q')
	template_data = dict()
	if keywords:
		query = SearchQuery(keywords)
		vector = SearchVector('title', 'content')
		qs = qs.annotate(search=vector).filter(search=query)
		qs = qs.annotate(rank=SearchRank(vector, query)).order_by('-rank')
		query = SearchQuery(keywords)
		vector = SearchVector('title')
		qsr = qsr.annotate(search=vector).filter(search=query)
		qsr = qsr.annotate(rank=SearchRank(vector, query)).order_by('-rank')
	template_data['post_list'] = qs
	template_data['resource_list'] = qsr
	return render(request, 'profiles/search.html', template_data)

def delete_resource(request, pk):
	resource = Resource.objects.filter(id=pk)
	resource.delete()
	return HttpResponseRedirect(request.META.get('HTTP_REFERER', '/'))

def recursive_delete(pk):
	folder = Folder.objects.filter(id=pk)
	resources = Resource.objects.filter(folder__in=folder)
	for resource in resources.all():
		resource.delete()
	folders = Folder.objects.filter(folder__in=folder)
	for f in folders.all():
		recursive_delete(f.id)
	folder.delete()

def delete_folder(request, pk):
	recursive_delete(pk)
	return HttpResponseRedirect(request.META.get('HTTP_REFERER', '/'))
	
