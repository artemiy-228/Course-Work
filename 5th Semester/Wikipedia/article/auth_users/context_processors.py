from auth_users.services import get_current_user_article_count


def auth_context(request) -> dict:
    if request.user.is_authenticated:
        return {
            'current_user_article_count': get_current_user_article_count(request.user),
        }
    return {}
