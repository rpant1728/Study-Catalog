# Generated by Django 2.1.2 on 2019-01-20 16:34

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('profiles', '0039_auto_20190120_0926'),
    ]

    operations = [
        migrations.AlterField(
            model_name='folder',
            name='parent_folder',
            field=models.ForeignKey(default=1, on_delete=django.db.models.deletion.CASCADE, related_name='subfolders', to='profiles.Folder'),
            preserve_default=False,
        ),
    ]
