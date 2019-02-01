# Generated by Django 2.1.4 on 2019-01-20 09:25

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('profiles', '0037_auto_20190120_0923'),
    ]

    operations = [
        migrations.AlterField(
            model_name='folder',
            name='parent_folder',
            field=models.ForeignKey(null=True, on_delete=django.db.models.deletion.CASCADE, related_name='subfolders', to='profiles.Folder'),
        ),
        migrations.AlterField(
            model_name='resource',
            name='course',
            field=models.ForeignKey(default=1, on_delete=django.db.models.deletion.CASCADE, related_name='resources', to='profiles.Course'),
            preserve_default=False,
        ),
    ]