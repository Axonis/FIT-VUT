@extends('layouts.app')

@section('content')
    <div class="container">
        <div class="row">
            <div class="col-md-8 col-md-offset-2">
                <div class="panel panel-default">
                    <div class="panel-heading">Ticket</div>

                    <div class="panel-body">
                        @if (session('status'))
                            <div class="alert alert-success">
                                {{ session('status') }}
                            </div>
                        @endif

                        @if (!$ticket)
                            <h2>This ticket does not exist!</h2>
                        @else
                            <form class="form-horizontal" role="form" method="POST"
                                  action="{{ action('TicketController@store_ticket') }}">
                                {{ csrf_field() }}

                                <div class="form-group">
                                    <label class="col-md-4 control-label">ID of ticket</label>
                                    <div class="col-md-6">
                                        <div class="radio">
                                            <input type="text" class="form-control" name="id"
                                                   value="{{$ticket->id_ticket}}" readonly>
                                        </div>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Ticket state</label>
                                    <div class="col-md-6">
                                        <select class="form-control" name="state"
                                                @if(Auth::user()->group != 'Admin' and Auth::user()->group != 'Programmer') disabled @endif
                                        >
                                            @foreach($ticket_types as $type)
                                                @if ($type == $ticket->stav)
                                                    <option selected>{{$type}}</option>
                                                @else
                                                    <option>{{$type}}</option>
                                                @endif
                                            @endforeach
                                        </select>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Created by</label>
                                    <div class="col-md-6">
                                        <div class="radio">
                                            <input id="fix_link" class="form-control" value="{{$ticket->name_vytvoreny}}"
                                                   readonly onclick="location.href='/profile/{{$ticket->name_vytvoreny}}'">
                                        </div>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Assigned to</label>
                                    <div class="col-md-6">
                                        <div class="radio">
                                            <input id="fix_link" class="form-control" value="{{$ticket->name_prideleny}}"
                                                   readonly onclick="location.href='/profile/{{$ticket->name_prideleny}}'">
                                        </div>
                                    </div>
                                </div>

                                <div class="form-group">
                                    <label class="col-md-4 control-label">Description</label>
                                    <div class="col-md-6">
                                        <textarea class="form-control" rows="15" name="desc"
                                                  @if(Auth::user()->group != 'Admin' and Auth::user()->group != 'Programmer') disabled @endif
                                        >{{$ticket->popis}}</textarea>
                                    </div>
                                </div>
                                <hr>

                                <label class="col-md-4 control-label" for="popis_bugu">Reporting bugs</label>
                                <div class="col-md-6">
                                <div class="table-responsive">
                                    <table class="table table-bordered" id="dataTable" width="100%" cellspacing="0">
                                        <thead>
                                        <tr>
                                            <th>Bug</th>
                                            <th>Type</th>
                                            <th>Severity</th>
                                            <th>Module</th>
                                            <th>Patch</th>
                                            <th>Description</th>
                                        </tr>
                                        </thead>

                                        <tbody>
                                        @foreach($bugs as $bug)
                                            <tr>
                                                <td class="col-md-1"><a
                                                            href="/bugs/{{$bug->id_bug}}"> {{ $bug->id_bug }} </a>
                                                </td>
                                                <td class="col-md-1"> {{ $bug->typ }} </td>
                                                <td class="col-md-1"> {{ $bug->zavaznost }} </td>
                                                <td class="col-md-1"> {{ $bug->meno }} </td>
                                                <td class="col-md-1"><a
                                                            href="/patches/{{$bug->id_patch}}">{{$bug->id_patch}}</a>
                                                </td>
                                                <td> {{ $bug->popis_bugu }} </td>
                                            </tr>
                                        @endforeach
                                        </tbody>
                                    </table>
                                    <hr>
                                    @if(Auth::user()->name == $ticket->name_vytvoreny)
                                        <button type="button" class="btn btn-default" id="fix_btn"
                                                onclick="location.href='/bugs/create'">Specify a new bug under this ticket</button>
                                    @endif
                                </div>
                                </div>
                                @if(Auth::user()->group == 'Admin' or Auth::user()->group == 'Programmer')
                                    <div class="col-md-6 col-md-offset-4">

                                        <div class="col-md-8">
                                        <button type="submit" name="submit" class="btn btn-primary">Submit changes
                                        </button>
                                        <button type="reset" class="btn btn-default">Reset fields</button>
                                        </div>
                                        <div class="col-md-4">
                                        @if(!$ticket->name_prideleny)
                                            <button type="submit" value="{{Auth::user()->name}}" name="accept"
                                                    class="btn btn-primary">Accept ticket
                                            </button>
                                        @endif

                                        @if(Auth::user()->name == $ticket->name_prideleny)
                                            <button type="submit" value="{{Auth::user()->name}}" name="reject"
                                                    class="btn btn-primary">Reject ticket
                                            </button>
                                        @endif
                                        </div>
                                    </div>
                                @endif

                            </form>
                        @endif

                    </div>
                </div>
            </div>
        </div>
    </div>
@endsection
