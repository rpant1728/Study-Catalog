# Generated by Django 2.1.1 on 2018-12-07 18:04

from django.conf import settings
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        migrations.swappable_dependency(settings.AUTH_USER_MODEL),
        ('profiles', '0005_comment'),
    ]

    operations = [
        migrations.AddField(
            model_name='post',
            name='upvote',
            field=models.ManyToManyField(related_name='upvotes', to=settings.AUTH_USER_MODEL),
        ),
        migrations.AddField(
            model_name='post',
            name='upvote_count',
            field=models.IntegerField(default=0),
        ),
    ]