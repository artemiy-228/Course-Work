from django.http import JsonResponse
from django.views import View
from django.shortcuts import render

from articles.services import get_hint, get_recent_articles, get_popular_articles


class HomeView(View):
    def get(self, request):
        recent_articles = get_recent_articles()
        popular_articles = get_popular_articles()
        return render(request, 'home.html', {'recent_articles': recent_articles, 'popular_articles': popular_articles})



def get_hint_view(request):
    return JsonResponse({'hint': get_hint()})

