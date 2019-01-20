# Generated by Django 2.1.4 on 2019-01-20 09:01

import datetime
from django.db import migrations, models
import django.db.models.deletion
from django.utils.timezone import utc


class Migration(migrations.Migration):

    dependencies = [
        ('profiles', '0035_auto_20190120_0859'),
    ]

    operations = [
        migrations.AddField(
            model_name='folder',
            name='course',
            field=models.ForeignKey(default=1, on_delete=django.db.models.deletion.CASCADE, related_name='folders', to='profiles.Course'),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='folder',
            name='created_on',
            field=models.DateTimeField(auto_now_add=True, default=datetime.datetime(2019, 1, 20, 9, 1, 16, 359081, tzinfo=utc)),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='folder',
            name='parent_folder',
            field=models.ForeignKey(default=1, on_delete=django.db.models.deletion.CASCADE, related_name='subfolders', to='profiles.Folder'),
            preserve_default=False,
        ),
        migrations.AddField(
            model_name='folder',
            name='root',
            field=models.BooleanField(default=False),
        ),
    ]
