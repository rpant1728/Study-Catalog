# Generated by Django 2.1.4 on 2018-12-19 13:43

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('profiles', '0013_course_resource'),
    ]

    operations = [
        migrations.RenameField(
            model_name='course',
            old_name='year',
            new_name='semester',
        ),
    ]
