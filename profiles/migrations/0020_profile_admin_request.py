# Generated by Django 2.1.1 on 2018-12-27 18:15

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('profiles', '0019_remove_course_semester'),
    ]

    operations = [
        migrations.AddField(
            model_name='profile',
            name='admin_request',
            field=models.BooleanField(default=False),
        ),
    ]
