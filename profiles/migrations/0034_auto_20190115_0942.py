# Generated by Django 2.1.1 on 2019-01-15 09:42

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('profiles', '0033_course_admin'),
    ]

    operations = [
        migrations.AlterField(
            model_name='folder',
            name='title',
            field=models.CharField(default='New Folder', max_length=200),
        ),
    ]
