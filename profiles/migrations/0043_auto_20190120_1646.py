# Generated by Django 2.1.2 on 2019-01-20 16:46

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('profiles', '0042_auto_20190120_1638'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='folder',
            name='course',
        ),
        migrations.RemoveField(
            model_name='folder',
            name='created_on',
        ),
        migrations.RemoveField(
            model_name='folder',
            name='parent_folder',
        ),
        migrations.RemoveField(
            model_name='folder',
            name='root',
        ),
    ]
