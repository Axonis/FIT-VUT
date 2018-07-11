@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Dashboard</div>

                    <div class="panel-body">
                        @if (!$bug)
                            <h2>This bug does not exist!</h2>
                        @else
                            <form class="form-horizontal" role="form" method="POST" action="{{ action('BugController@store') }}">
                                {{ csrf_field() }}

                                @if ($errors->any())
                                    <div class="alert alert-danger">
                                        <ul>
                                            @foreach ($errors->all() as $error)
                                                <li>{{ $error }}</li>
                                            @endforeach
                                        </ul>
                                    </div>
                                @endif

                                <div class="form-group">
                                    <label class="col-md-4 control-label">ID of bug</label>
                                    <div class="col-md-6">
                                    <div class="radio">
                                        <input type="text" class="form-control" name="id" value="{{$bug->id_bug}}" readonly>
                                    </div>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Severity of a bug</label>
                                    <div class="col-md-6">
                                    @foreach($severity as $item)
                                        <div class="radio">
                                            <label>
                                                <input type="radio" name="zavaznost" id="{{$item}}" value="{{$item}}"
                                                @if ($item == $bug->zavaznost) checked @endif
                                                >
                                                {{$item}}
                                            </label>
                                        </div>
                                    @endforeach
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Type of a bug</label>
                                    <div class="col-md-6">
                                    <select class="form-control" name="typ"
                                    @if(Auth::user()->group != 'Admin' and Auth::user()->group != 'Programmer')
                                            disabled
                                    @endif
                                    >
                                        @foreach($bug_types as $type)
                                            @if ($type == $bug->typ)
                                                <option selected>{{$type}}</option>
                                            @else
                                                <option>{{$type}}</option>
                                            @endif
                                        @endforeach
                                    </select>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Bug is assigned to</label>
                                    <div class="col-md-6">
                                    <select class="form-control" name="typ"
                                            @if(Auth::user()->group != 'Admin' and Auth::user()->group != 'Programmer')
                                            disabled
                                            @endif
                                    >
                                        @foreach($modules as $module)
                                            @if ($module->id_modul == $bug->id_modul)
                                                <option selected>{{$module->meno}}</option>
                                            @else
                                                <option>{{$module->meno}}</option>
                                            @endif
                                        @endforeach
                                    </select>
                                    </div>
                                </div>

                                @if($vulnerab)
                                <div class="form-group">
                                    <label class="col-md-4 control-label">Vulnerability reported</label>
                                    <div class="col-md-6">
                                        <select class="form-control" name="miera_nebezpecenstva"
                                                @if(Auth::user()->group != 'Admin' and Auth::user()->group != 'Programmer')
                                                disabled
                                                @endif
                                        >
                                            @foreach($vuln as $item)

                                                @if ($item == $vulnerab->miera_nebezpecenstva)
                                                    <option selected>{{$vulnerab->miera_nebezpecenstva}}</option>
                                                @else
                                                    <option>{{$item}}</option>
                                                @endif
                                            @endforeach
                                        </select>
                                    </div>
                                </div>
                                @endif

                                <div class="form-group">
                                    <label class="col-md-4 control-label" for="popis_bugu">Description of Bug</label>
                                    <div class="col-md-6">
                                    <textarea name="popis_problemu" class="form-control" rows="3" id="popis_bugu"
                                              placeholder="Preferably what is the bug doing, how is reproducible, etc."
                                              @if(Auth::user()->group != 'Admin' and Auth::user()->group != 'Programmer') readonly @endif
                                    >{{$bug->popis_bugu}}</textarea>
                                </div>
                                </div>

                                @if(Auth::user()->group == 'Admin' or Auth::user()->group == 'Programmer')
                                    <div class="col-md-6 col-md-offset-4">
                                    <button type="submit" class="btn btn-primary">Submit Button</button>
                                    <button type="reset" class="btn btn-default">Reset Button</button>
                                    </div>
                                @endif
                                @if(Auth::user()->group == 'Admin' or Auth::user()->group == 'Programmer')
                                @endif

                            </form>
                        @endif

                    </div>
                </div>
            </div>
        </div>
    </div>
@endsection
