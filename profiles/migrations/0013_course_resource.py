# Generated by Django 2.1.4 on 2018-12-19 13:38

from django.conf import settings
from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('profiles', '0012_auto_20181211_1537'),
    ]

    operations = [
        migrations.CreateModel(
            name='Course',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('department', models.CharField(blank=True, choices=[('CSE', 'Computer Science and Engineering'), ('CE', 'Chemical Engineering'), ('BT', 'Bio-Technology'), ('CST', 'Chemical Science & Technology'), ('CE', 'Civil Engineering'), ('ECE', 'Electronics & Communication Engineering'), ('EEE', 'Electronics & Electrical Engineering'), ('EP', 'Engineering Physics'), ('ME', 'Mechanical Engineering'), ('MNC', 'Mathematics & Computing')], help_text='Department', max_length=3)),
                ('degree', models.CharField(blank=True, choices=[('PhD', 'Doctorate of Philosophy'), ('B.Tech', 'Bachelor of Technology'), ('M.Tech', 'Master of Technology'), ('B.Des', 'Bachelor of Design'), ('M.Des', 'Master of Design')], help_text='Degree', max_length=6)),
                ('name', models.CharField(max_length=200)),
                ('code', models.CharField(max_length=20)),
                ('year', models.IntegerField()),
            ],
        ),
        migrations.CreateModel(
            name='Resource',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('file', models.FileField(upload_to='resources/')),
                ('description', models.CharField(max_length=300)),
                ('uploaded_on', models.DateTimeField(auto_now_add=True)),
                ('course', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='course', to='profiles.Course')),
                ('user', models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='user', to=settings.AUTH_USER_MODEL)),
            ],
        ),
    ]