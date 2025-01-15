from articles.models import  *
from articles.services import *


def global_context(request) -> dict:


    return {
        'article_count': get_articles_count(),
        'users_count': get_users_count(),
        'article_name': get_hint(),
        'recent_articles': get_recent_articles(),
        'popular_articles': get_popular_articles(),
    }