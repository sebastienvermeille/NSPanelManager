# Generated by Django 4.2.6 on 2023-11-25 16:11

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('web', '0036_relaygroupbinding_relaygroup'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='relaygroup',
            name='relay_group_bindings',
        ),
        migrations.AddField(
            model_name='relaygroupbinding',
            name='relay_group',
            field=models.ForeignKey(default=None, null=True, on_delete=django.db.models.deletion.CASCADE, to='web.relaygroup'),
        ),
    ]