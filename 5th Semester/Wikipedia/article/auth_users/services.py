from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User

from articles.models import Article


def get_current_user_article_count(user: User) -> int:
    return Article.objects.filter(author=user).count()
