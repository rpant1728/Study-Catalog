# Generated by Django 2.1.1 on 2018-12-10 13:57

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('profiles', '0009_post_post_id'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='post',
            name='post_id',
        ),
    ]
