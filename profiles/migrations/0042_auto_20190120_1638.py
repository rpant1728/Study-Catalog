# Generated by Django 2.1.2 on 2019-01-20 16:38

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('profiles', '0041_auto_20190120_1637'),
    ]

    operations = [
        migrations.AlterField(
            model_name='folder',
            name='parent_folder',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='subfolders', to='profiles.Folder'),
        ),
    ]
