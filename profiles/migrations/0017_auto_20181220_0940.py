# Generated by Django 2.1.4 on 2018-12-20 09:40

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('profiles', '0016_auto_20181220_0929'),
    ]

    operations = [
        migrations.AlterField(
            model_name='course',
            name='code',
            field=models.CharField(blank=True, max_length=20),
        ),
        migrations.AlterField(
            model_name='course',
            name='department',
            field=models.CharField(blank=True, choices=[('CSE', 'Computer Science and Engineering'), ('CE', 'Chemical Engineering'), ('BT', 'Bio-Technology'), ('CST', 'Chemical Science & Technology'), ('CE', 'Civil Engineering'), ('ECE', 'Electronics & Communication Engineering'), ('EEE', 'Electronics & Electrical Engineering'), ('EP', 'Engineering Physics'), ('ME', 'Mechanical Engineering'), ('MNC', 'Mathematics & Computing'), ('OT', 'Other')], help_text='Department', max_length=3),
        ),
        migrations.AlterField(
            model_name='course',
            name='semester',
            field=models.IntegerField(blank=True),
        ),
        migrations.AlterField(
            model_name='profile',
            name='department',
            field=models.CharField(blank=True, choices=[('CSE', 'Computer Science and Engineering'), ('CE', 'Chemical Engineering'), ('BT', 'Bio-Technology'), ('CST', 'Chemical Science & Technology'), ('CE', 'Civil Engineering'), ('ECE', 'Electronics & Communication Engineering'), ('EEE', 'Electronics & Electrical Engineering'), ('EP', 'Engineering Physics'), ('ME', 'Mechanical Engineering'), ('MNC', 'Mathematics & Computing'), ('OT', 'Other')], help_text='Department', max_length=3),
        ),
    ]
