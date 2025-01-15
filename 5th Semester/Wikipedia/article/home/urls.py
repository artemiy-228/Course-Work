from django.urls import path
from . import views

urlpatterns = [
    path('', views.HomeView.as_view(), name='home'),
    path('get-hint/', views.get_hint_view, name='get_hint'),
]