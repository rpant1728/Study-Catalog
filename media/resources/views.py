from django.shortcuts import render, get_object_or_404
from django.template.loader import get_template
from django.http import HttpResponse, HttpResponseRedirect
from.forms import ImageUploadForm
from django.core.mail import send_mail
from django.template import Context
from . import models
from .models import User, Education, WorkExperience
from django.db import models


def loginpage(request):
    t = get_template('login/index.html')
    html = t.render({}, request)
    return HttpResponse(html)


def signup(request):
    name = request.POST['name']
    webmail = request.POST['webmail']
    passwd = request.POST['pass']
    cpasswd = request.POST['cpasswd']
    if passwd != cpasswd:
        t = get_template('login/index.html')
        html = t.render({}, request)
        return HttpResponse(html)
    elif passwd == cpasswd:
        check = User.objects.filter(webmail=webmail)
        if check.exists():
            t = get_template('login/index.html')
            html = t.render({}, request)
            return HttpResponse(html)
        else:
            obj = User(name=name, webmail=webmail, password=passwd)
            obj.save()
            msg = 'registered'
            return HttpResponse(msg)


def portal(request):
    webmail = request.session['faculty_mail']
    check = get_object_or_404(User, webmail=webmail)
    return render(request, 'portal/index.html', {'user': check})


def signin(request):
    webmail = request.POST['webmail']
    check = User.objects.filter(webmail=webmail).values('password')
    if check.exists():
        passwd = request.POST['password']
        if check[0]['password'] == passwd:
            request.session['faculty_mail'] = webmail
            return HttpResponseRedirect('/ssl_main/portal/')
        else:
            t = get_template('login/index.html')
            html = t.render({}, request)
            return HttpResponse(html)
    else:
        t = get_template('login/index.html')
        html = t.render({}, request)
        return HttpResponse(html)


def updateinfo(request):
    name = request.POST['name']
    dept = request.POST['dept']
    desg = request.POST['desg']
    resadd = request.POST['resadd']
    resnum = request.POST['resnum']
    offadd = request.POST['offadd']
    offnum = request.POST['offnum']
    faculty = User.objects.filter(webmail=request.session['faculty_mail'])
    faculty.update(
        name=name,
        department=dept,
        designation=desg,
        residence=resadd,
        residence_phone_num=resnum,
        office_room_num=offadd,
        office_phone_ext=offnum,
    )
    return HttpResponseRedirect('/ssl_main/portal/')


def changepass(request):
    opass = request.POST['opass']
    npass = request.POST['npass']
    cnpass = request.POST['cnpass']
    faculty = User.objects.filter(webmail=request.session['faculty_mail']).values('password')
    if faculty[0]['password'] == opass:
        if cnpass == npass:
            User.objects.filter(webmail=request.session['faculty_mail']).update(password=npass)

    return HttpResponseRedirect('/ssl_main/portal/')


def eduandwork(request):
    webmail = request.session['faculty_mail']
    check = get_object_or_404(User, webmail=webmail)
    return render(request, 'portal/eduandwork.html', {'user': check})


def uploadphoto(request):
    webmail = request.session['faculty_mail']
    check = get_object_or_404(User, webmail=webmail)
    return render(request, 'portal/uploadphoto.html', {'user': check})


def addedu(request):
    course = request.POST['course']
    institution = request.POST['institution']
    field = request.POST['field']
    start = request.POST['start']
    end = request.POST['end']
    faculty = User.objects.filter(webmail=request.session['faculty_mail'])
    if faculty.exists():
        obj = Education(course=course, institution=institution, field=field, start_year=start, end_year=end, user=faculty[0])
        obj.save()
    return HttpResponseRedirect('/ssl_main/portal/eduandwork/')


def addwork(request):
    org = request.POST['org']
    start = request.POST['start']
    end = request.POST['end']
    desg = request.POST['desg']
    faculty = User.objects.filter(webmail=request.session['faculty_mail'])
    if faculty.exists():
        obj = WorkExperience(organisation=org, start_year=start, end_year=end, designation=desg, user=faculty[0])
        obj.save()
    return HttpResponseRedirect('/ssl_main/portal/eduandwork/')


def upload(request):
    faculty = User.objects.filter(webmail=request.session['faculty_mail'])
    imgvalid = ImageUploadForm(request.POST, request.FILES)
    if faculty.exists():
        if imgvalid.is_valid():
            User.objects.filter(webmail=request.session['faculty_mail']).save(img=imgvalid.cleaned_data["picture"])
        else:
            return HttpResponse("Gadbad hai beta")
    return HttpResponseRedirect('/ssl_main/portal/uploadphoto/')