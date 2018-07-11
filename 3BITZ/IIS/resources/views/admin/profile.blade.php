@extends('layouts.app')
@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Edit profile</div>

                    <div class="panel-body">

                        <form class="form-horizontal" role="form" method="POST" action="{{ action('AdminController@store') }}">
                            {{ csrf_field() }}


                            <div class="form-group{{ $errors->has('name') ? ' has-error' : '' }}">
                                <label for="name" class="col-md-4 control-label">Username</label>

                                <div class="col-md-6">
                                    <input id="name" type="text" class="form-control" name="name"
                                           value={{$aux->name}} required autofocus>

                                    @if ($errors->has('name'))
                                        <span class="help-block">
                                        <strong>{{ $errors->first('name') }}</strong>
                                    </span>
                                    @endif
                                </div>
                            </div>

                            @if ($profile->group == 'Programmer')
                                <div class="form-group">
                                    <label for="rank" class="col-md-4 control-label">Rank</label>

                                    <div class="col-md-6">
                                        <input id="rank" type="number" class="form-control" name="rank"
                                               value={{$aux->rank}}>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Product</label>
                                    <div class="col-md-6">
                                    <select class="form-control" name="produkt">
                                        @foreach($produkty as $produkt)
                                            @if ($produkt == $aux->produkt)
                                                <option selected>{{$produkt}}</option>
                                            @else
                                                <option>{{$produkt}}</option>
                                            @endif
                                        @endforeach
                                    </select>
                                    </div>
                                </div>
                            @endif

                            @if ($profile->group == 'User')
                                <div class="panel-group" id="accordion">
                                    <div class="panel panel-default">
                                        <div class="panel-heading">
                                            <h4 class="panel-title">
                                                <a data-toggle="collapse" data-parent="#accordion" href="#collapse1">
                                                    Information about user</a>
                                            </h4>
                                        </div>
                                        <div id="collapse1" class="panel-collapse collapse in">
                                            <div class="form-group">
                                                <label for="karma" class="col-md-4 control-label">Karma</label>

                                                <div class="col-md-6">
                                                    <input id="karma" type="text" class="form-control" name="karma"
                                                           value={{$aux->karma}}>
                                                </div>
                                            </div>

                                            <div class="form-group">
                                                <label for="odmena" class="col-md-4 control-label">Reward</label>

                                                <div class="col-md-6">
                                                    <input id="odmena" type="text" class="form-control" name="odmena"
                                                           value={{$aux->odmena}}>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                    <div class="panel panel-default">
                                        <div class="panel-heading">
                                            <h4 class="panel-title">
                                                <a data-toggle="collapse" data-parent="#accordion" href="#collapse2">
                                                    Make user a programmer</a>
                                            </h4>
                                        </div>
                                        <div id="collapse2" class="panel-collapse collapse">
                                                <div class="form-group">
                                                    <label for="rank" class="col-md-4 control-label">Rank</label>

                                                    <div class="col-md-6">
                                                        <input id="rank" type="number" class="form-control" name="rank">
                                                    </div>
                                                </div>

                                            <div class="form-group">
                                                <label class="col-md-4 control-label">Product</label>
                                                <div class="col-md-6">
                                                    <select class="form-control" name="produkt">
                                                        @foreach($produkty as $produkt)
                                                            <option>{{$produkt}}</option>
                                                        @endforeach
                                                    </select>
                                                </div>
                                            </div>
                                        </div>
                                    </div>
                                </div>


                            @endif

                            <div class="form-group">
                                <div class="col-md-6 col-md-offset-4">
                                    <button type="submit" class="btn btn-primary">
                                        Submit changes
                                    </button>
                                    <button type="submit" class="btn btn-default">
                                        Reset changes
                                    </button>
                                </div>
                            </div>

                            </form>

                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
@endsection
